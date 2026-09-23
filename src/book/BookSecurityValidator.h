#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

namespace EtapKitap {

class BookSecurityValidator {
public:
    static constexpr qint64 MAX_SINGLE_FILE_SIZE = 500 * 1024 * 1024;    // 500 MB (for large HD videos)
    static constexpr qint64 MAX_PACKAGE_UNCOMPRESSED_SIZE = 2 * 1024 * 1024 * 1024LL; // 2 GB
    static constexpr double MAX_COMPRESSION_RATIO = 100.0; // Zip bomb mitigation

    /**
     * @brief Checks if a relative path within the package tries to escape via directory traversal.
     * Rejects paths starting with '/', containing '..', backslashes or absolute drive prefixes.
     */
    [[nodiscard]] static bool isSafeRelativePath(const QString& relPath);

    /**
     * @brief Checks if file extension is an allowed media/asset type.
     * Rejects executables, scripts, binaries (.exe, .sh, .bat, .dll, .so, etc.)
     */
    [[nodiscard]] static bool isAllowedAssetExtension(const QString& filePath);

    /**
     * @brief Checks if an uncompressed size is within acceptable bounds.
     */
    [[nodiscard]] static bool isSafeFileSize(qint64 uncompressedSize, qint64 compressedSize);
};

} // namespace EtapKitap
