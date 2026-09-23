#include "book/BookSecurityValidator.h"
#include <QtCore/QFileInfo>
#include <QtCore/QSet>

namespace EtapKitap {

bool BookSecurityValidator::isSafeRelativePath(const QString& relPath) {
    if (relPath.isEmpty()) {
        return false;
    }

    // Must not be absolute
    if (relPath.startsWith('/') || relPath.startsWith('\\') || (relPath.size() > 1 && relPath[1] == ':')) {
        return false;
    }

    // Split components and check for traversal
    QString normalized = relPath;
    normalized.replace('\\', '/');

    const QStringList parts = normalized.split('/', Qt::SkipEmptyParts);
    for (const auto& part : parts) {
        if (part == QStringLiteral("..") || part == QStringLiteral(".")) {
            return false;
        }
        // Disallow dangerous control characters
        for (const auto& ch : part) {
            if (ch.unicode() < 32 || ch.unicode() == 127) {
                return false;
            }
        }
    }

    return true;
}

bool BookSecurityValidator::isAllowedAssetExtension(const QString& filePath) {
    QFileInfo fi(filePath);
    QString ext = fi.suffix().toLower();

    static const QSet<QString> allowedExtensions = {
        // Documents & Data
        QStringLiteral("pdf"), QStringLiteral("json"), QStringLiteral("xml"), QStringLiteral("txt"),
        // Images
        QStringLiteral("png"), QStringLiteral("jpg"), QStringLiteral("jpeg"), QStringLiteral("svg"),
        QStringLiteral("webp"), QStringLiteral("gif"),
        // Audio
        QStringLiteral("mp3"), QStringLiteral("ogg"), QStringLiteral("wav"), QStringLiteral("m4a"),
        QStringLiteral("aac"), QStringLiteral("flac"),
        // Video
        QStringLiteral("mp4"), QStringLiteral("webm"), QStringLiteral("ogv"), QStringLiteral("mkv"),
        // 3D & Models (future-proof)
        QStringLiteral("gltf"), QStringLiteral("glb")
    };

    return allowedExtensions.contains(ext);
}

bool BookSecurityValidator::isSafeFileSize(qint64 uncompressedSize, qint64 compressedSize) {
    if (uncompressedSize < 0 || uncompressedSize > MAX_SINGLE_FILE_SIZE) {
        return false;
    }

    if (compressedSize > 0) {
        double ratio = static_cast<double>(uncompressedSize) / static_cast<double>(compressedSize);
        if (ratio > MAX_COMPRESSION_RATIO && uncompressedSize > 1024 * 1024) {
            return false; // Potential zip bomb
        }
    }

    return true;
}

} // namespace EtapKitap
