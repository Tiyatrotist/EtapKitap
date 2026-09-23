#pragma once

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QMap>

namespace EtapKitap {

class ZipExtractor {
public:
    /**
     * @brief Extracts a ZIP file to the target destination directory.
     * Enforces path traversal checks, size limits, and safe permissions.
     */
    static bool extract(const QString& zipFilePath, const QString& destinationDir, QString* outError = nullptr);

    /**
     * @brief Reads a single file from a ZIP archive into memory.
     */
    static QByteArray readFile(const QString& zipFilePath, const QString& relativeFileName, QString* outError = nullptr);
};

} // namespace EtapKitap
