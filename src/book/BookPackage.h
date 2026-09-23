#pragma once

#include "book/BookManifest.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <memory>

namespace EtapKitap {

class BookPackage : public QObject {
    Q_OBJECT

public:
    explicit BookPackage(QObject* parent = nullptr);
    ~BookPackage() override;

    bool open(const QString& packagePath, QString* outError = nullptr);
    void close();
    [[nodiscard]] bool isOpen() const;

    [[nodiscard]] const BookManifest& manifest() const;
    [[nodiscard]] QString packagePath() const;
    [[nodiscard]] QString pdfDocumentPath() const;
    [[nodiscard]] QByteArray readAssetData(const QString& relPath) const;
    [[nodiscard]] QUrl resolveAssetUrl(const QString& relPath) const;
    [[nodiscard]] QString resolveAssetFilePath(const QString& relPath) const;

private:
    bool loadFromDirectory(const QString& dirPath, QString* outError);
    bool extractAndLoadZip(const QString& zipPath, QString* outError);

    bool m_isOpen{false};
    QString m_packagePath;
    QString m_extractedDir;
    BookManifest m_manifest;
};

} // namespace EtapKitap
