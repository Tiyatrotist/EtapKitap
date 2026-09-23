#include "book/BookPackage.h"
#include "book/ZipExtractor.h"
#include "book/BookSecurityValidator.h"
#include "core/ApplicationContext.h"
#include "core/Logger.h"
#include <QtCore/QFileInfo>
#include <QtCore/QCryptographicHash>

namespace EtapKitap {

BookPackage::BookPackage(QObject* parent)
    : QObject(parent)
{
}

BookPackage::~BookPackage() {
    close();
}

bool BookPackage::open(const QString& packagePath, QString* outError) {
    close();

    QFileInfo fi(packagePath);
    if (!fi.exists()) {
        if (outError) *outError = QStringLiteral("Package file does not exist: ") + packagePath;
        return false;
    }

    m_packagePath = packagePath;

    if (fi.isDir()) {
        return loadFromDirectory(packagePath, outError);
    } else {
        return extractAndLoadZip(packagePath, outError);
    }
}

void BookPackage::close() {
    m_isOpen = false;
    m_packagePath.clear();
    m_extractedDir.clear();
    m_manifest = BookManifest();
}

bool BookPackage::isOpen() const {
    return m_isOpen;
}

const BookManifest& BookPackage::manifest() const {
    return m_manifest;
}

QString BookPackage::packagePath() const {
    return m_packagePath;
}

QString BookPackage::pdfDocumentPath() const {
    if (!m_isOpen || m_extractedDir.isEmpty()) {
        return QString();
    }
    return QDir(m_extractedDir).filePath(m_manifest.documentPdf);
}

QByteArray BookPackage::readAssetData(const QString& relPath) const {
    if (!m_isOpen || !BookSecurityValidator::isSafeRelativePath(relPath)) {
        return QByteArray();
    }

    QString fullPath = resolveAssetFilePath(relPath);
    QFile f(fullPath);
    if (f.open(QIODevice::ReadOnly)) {
        return f.readAll();
    }
    return QByteArray();
}

QUrl BookPackage::resolveAssetUrl(const QString& relPath) const {
    QString p = resolveAssetFilePath(relPath);
    if (p.isEmpty()) return QUrl();
    return QUrl::fromLocalFile(p);
}

QString BookPackage::resolveAssetFilePath(const QString& relPath) const {
    if (!m_isOpen || !BookSecurityValidator::isSafeRelativePath(relPath)) {
        return QString();
    }
    return QDir(m_extractedDir).filePath(relPath);
}

bool BookPackage::loadFromDirectory(const QString& dirPath, QString* outError) {
    m_extractedDir = dirPath;
    QString manifestPath = QDir(dirPath).filePath(QStringLiteral("manifest.json"));
    QFile mf(manifestPath);
    if (!mf.open(QIODevice::ReadOnly)) {
        if (outError) *outError = QStringLiteral("Cannot find manifest.json in package directory.");
        return false;
    }

    QByteArray data = mf.readAll();
    m_manifest = BookManifest::fromJson(data, outError);
    if (!m_manifest.isValid()) {
        return false;
    }

    m_isOpen = true;
    qCInfo(logBook) << "Opened interactive book from directory:" << m_manifest.title << "ID:" << m_manifest.id;
    return true;
}

bool BookPackage::extractAndLoadZip(const QString& zipPath, QString* outError) {
    QByteArray hash = QCryptographicHash::hash(zipPath.toUtf8(), QCryptographicHash::Sha256).toHex().left(16);
    QString targetDir = QDir(ApplicationContext::instance().cacheDirectory()).filePath(QStringLiteral("packages/") + QString::fromLatin1(hash));

    QDir().mkpath(targetDir);

    QString manifestPath = QDir(targetDir).filePath(QStringLiteral("manifest.json"));
    // If not already extracted or timestamp changed
    if (!QFile::exists(manifestPath)) {
        if (!ZipExtractor::extract(zipPath, targetDir, outError)) {
            return false;
        }
    }

    return loadFromDirectory(targetDir, outError);
}

} // namespace EtapKitap
