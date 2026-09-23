#include "book/ZipExtractor.h"
#include "book/BookSecurityValidator.h"
#include "core/Logger.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDataStream>

#if __has_include(<zlib.h>)
#include <zlib.h>
#elif __has_include(<QtZlib/zlib.h>)
#include <QtZlib/zlib.h>
#endif

namespace EtapKitap {

#pragma pack(push, 1)
struct LocalFileHeader {
    quint32 signature;       // 0x04034b50
    quint16 versionNeeded;
    quint16 generalPurpose;
    quint16 compressionMethod;
    quint16 lastModTime;
    quint16 lastModDate;
    quint32 crc32;
    quint32 compressedSize;
    quint32 uncompressedSize;
    quint16 fileNameLength;
    quint16 extraFieldLength;
};

struct CentralDirHeader {
    quint32 signature;       // 0x02014b50
    quint16 versionMadeBy;
    quint16 versionNeeded;
    quint16 generalPurpose;
    quint16 compressionMethod;
    quint16 lastModTime;
    quint16 lastModDate;
    quint32 crc32;
    quint32 compressedSize;
    quint32 uncompressedSize;
    quint16 fileNameLength;
    quint16 extraFieldLength;
    quint16 fileCommentLength;
    quint16 diskNumberStart;
    quint16 internalFileAttributes;
    quint32 externalFileAttributes;
    quint32 relativeOffsetOfLocalHeader;
};
#pragma pack(pop)

static bool inflateRaw(const QByteArray& compressed, QByteArray& outUncompressed, quint32 expectedSize) {
#if defined(ZLIB_VERSION) || defined(ZLIB_H)
    outUncompressed.resize(static_cast<qsizetype>(expectedSize));

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(compressed.data()));
    strm.avail_in = static_cast<uInt>(compressed.size());
    strm.next_out = reinterpret_cast<Bytef*>(outUncompressed.data());
    strm.avail_out = static_cast<uInt>(outUncompressed.size());

    // -MAX_WBITS tells zlib to expect a raw DEFLATE stream without zlib/gzip headers
    if (inflateInit2(&strm, -MAX_WBITS) != Z_OK) {
        return false;
    }

    int ret = inflate(&strm, Z_FINISH);
    inflateEnd(&strm);

    return (ret == Z_STREAM_END || ret == Z_OK);
#else
    Q_UNUSED(compressed);
    Q_UNUSED(outUncompressed);
    Q_UNUSED(expectedSize);
    return false;
#endif
}

bool ZipExtractor::extract(const QString& zipFilePath, const QString& destinationDir, QString* outError) {
    QFile file(zipFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (outError) *outError = QStringLiteral("Cannot open ZIP file: ") + file.errorString();
        return false;
    }

    QDir dest(destinationDir);
    if (!dest.exists()) {
        dest.mkpath(QStringLiteral("."));
    }

    qint64 totalExtractedSize = 0;

    while (!file.atEnd()) {
        qint64 headerPos = file.pos();
        quint32 sig = 0;
        if (file.read(reinterpret_cast<char*>(&sig), sizeof(sig)) != sizeof(sig)) {
            break;
        }

        if (sig != 0x04034b50) {
            // Not a local file header (reached central directory or end)
            break;
        }

        file.seek(headerPos);
        LocalFileHeader header;
        if (file.read(reinterpret_cast<char*>(&header), sizeof(header)) != sizeof(header)) {
            break;
        }

        QByteArray fileNameBytes = file.read(header.fileNameLength);
        QString fileName = QString::fromUtf8(fileNameBytes);
        file.seek(file.pos() + header.extraFieldLength); // Skip extra field

        // Security check: validate relative path
        if (!BookSecurityValidator::isSafeRelativePath(fileName)) {
            if (outError) *outError = QStringLiteral("Path traversal or illegal file name in package: ") + fileName;
            return false;
        }

        // Security check: single file size & compression ratio
        if (!BookSecurityValidator::isSafeFileSize(header.uncompressedSize, header.compressedSize)) {
            if (outError) *outError = QStringLiteral("Zip entry violates maximum size or compression limits: ") + fileName;
            return false;
        }

        totalExtractedSize += header.uncompressedSize;
        if (totalExtractedSize > BookSecurityValidator::MAX_PACKAGE_UNCOMPRESSED_SIZE) {
            if (outError) *outError = QStringLiteral("Package exceeds total uncompressed size limit.");
            return false;
        }

        // Directory entry
        if (fileName.endsWith('/') || fileName.endsWith('\\')) {
            dest.mkpath(fileName);
            continue;
        }

        QByteArray compressedData = file.read(header.compressedSize);
        QByteArray uncompressedData;

        if (header.compressionMethod == 0) { // Stored
            uncompressedData = compressedData;
        } else if (header.compressionMethod == 8) { // Deflated
            if (!inflateRaw(compressedData, uncompressedData, header.uncompressedSize)) {
                if (outError) *outError = QStringLiteral("Failed to decompress entry: ") + fileName;
                return false;
            }
        } else {
            if (outError) *outError = QStringLiteral("Unsupported ZIP compression method: ") + QString::number(header.compressionMethod);
            return false;
        }

        // Create parent folder if needed
        QString targetFilePath = dest.filePath(fileName);
        QFileInfo fi(targetFilePath);
        dest.mkpath(fi.dir().path());

        QFile outFile(targetFilePath);
        if (!outFile.open(QIODevice::WriteOnly)) {
            if (outError) *outError = QStringLiteral("Failed to write extracted file: ") + targetFilePath;
            return false;
        }
        outFile.write(uncompressedData);
        outFile.close();
    }

    return true;
}

QByteArray ZipExtractor::readFile(const QString& zipFilePath, const QString& relativeFileName, QString* outError) {
    QFile file(zipFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (outError) *outError = QStringLiteral("Cannot open ZIP file: ") + file.errorString();
        return QByteArray();
    }

    while (!file.atEnd()) {
        qint64 headerPos = file.pos();
        quint32 sig = 0;
        if (file.read(reinterpret_cast<char*>(&sig), sizeof(sig)) != sizeof(sig)) {
            break;
        }

        if (sig != 0x04034b50) {
            break;
        }

        file.seek(headerPos);
        LocalFileHeader header;
        if (file.read(reinterpret_cast<char*>(&header), sizeof(header)) != sizeof(header)) {
            break;
        }

        QByteArray fileNameBytes = file.read(header.fileNameLength);
        QString fileName = QString::fromUtf8(fileNameBytes);
        file.seek(file.pos() + header.extraFieldLength);

        QByteArray compressedData = file.read(header.compressedSize);

        if (fileName == relativeFileName) {
            QByteArray uncompressedData;
            if (header.compressionMethod == 0) {
                return compressedData;
            } else if (header.compressionMethod == 8) {
                if (inflateRaw(compressedData, uncompressedData, header.uncompressedSize)) {
                    return uncompressedData;
                }
            }
            if (outError) *outError = QStringLiteral("Decompression error for file: ") + relativeFileName;
            return QByteArray();
        }
    }

    if (outError) *outError = QStringLiteral("File not found in ZIP archive: ") + relativeFileName;
    return QByteArray();
}

} // namespace EtapKitap
