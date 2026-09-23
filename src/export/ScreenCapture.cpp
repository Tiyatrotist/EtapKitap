#include "export/ScreenCapture.h"
#include "core/Logger.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>

namespace EtapKitap {

ScreenCapture::ScreenCapture(QObject* parent)
    : QObject(parent)
{
}

bool ScreenCapture::copyToClipboard(const QImage& image) {
    if (image.isNull()) return false;
    QClipboard* clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setImage(image);
        qCDebug(logCore) << "Copied image to clipboard (" << image.width() << "x" << image.height() << ")";
        return true;
    }
    return false;
}

bool ScreenCapture::saveToFile(const QImage& image, const QString& filePath) {
    if (image.isNull() || filePath.isEmpty()) return false;
    bool success = image.save(filePath);
    if (success) {
        qCInfo(logCore) << "Saved screenshot to:" << filePath;
    } else {
        qCWarning(logCore) << "Failed to save screenshot to:" << filePath;
    }
    return success;
}

QImage ScreenCapture::cropRegion(const QImage& source, const QRectF& normalizedRect) {
    if (source.isNull()) return QImage();

    int x = static_cast<int>(normalizedRect.x() * source.width());
    int y = static_cast<int>(normalizedRect.y() * source.height());
    int w = static_cast<int>(normalizedRect.width() * source.width());
    int h = static_cast<int>(normalizedRect.height() * source.height());

    QRect cropRect(x, y, w, h);
    cropRect = cropRect.intersected(source.rect());
    return source.copy(cropRect);
}

} // namespace EtapKitap
