#pragma once

#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtCore/QRectF>

namespace EtapKitap {

class ScreenCapture : public QObject {
    Q_OBJECT

public:
    explicit ScreenCapture(QObject* parent = nullptr);
    ~ScreenCapture() override = default;

    Q_INVOKABLE bool copyToClipboard(const QImage& image);
    Q_INVOKABLE bool saveToFile(const QImage& image, const QString& filePath);
    [[nodiscard]] static QImage cropRegion(const QImage& source, const QRectF& normalizedRect);
};

} // namespace EtapKitap
