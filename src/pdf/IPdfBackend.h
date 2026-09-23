#pragma once

#include <QtCore/QString>
#include <QtCore/QSizeF>
#include <QtGui/QImage>
#include <memory>

namespace EtapKitap {

/**
 * @brief Interface for PDF rendering and metadata extraction backends.
 */
class IPdfBackend {
public:
    virtual ~IPdfBackend() = default;

    [[nodiscard]] virtual bool load(const QString& filePath) = 0;
    virtual void close() = 0;
    [[nodiscard]] virtual bool isLoaded() const = 0;

    [[nodiscard]] virtual int pageCount() const = 0;
    [[nodiscard]] virtual QSizeF pageSize(int pageNumber) const = 0;
    [[nodiscard]] virtual QImage renderPage(int pageNumber, const QSize& targetPixelSize) = 0;
    [[nodiscard]] virtual QString pageText(int pageNumber) const = 0;
    [[nodiscard]] virtual QString title() const = 0;
};

} // namespace EtapKitap
