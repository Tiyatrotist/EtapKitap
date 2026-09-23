#pragma once

#include "document/DocumentController.h"
#include <QtQuick/QQuickImageProvider>

namespace EtapKitap {

class PageImageProvider : public QQuickImageProvider {
public:
    explicit PageImageProvider(DocumentController* controller);
    ~PageImageProvider() override = default;

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    DocumentController* m_controller;
};

} // namespace EtapKitap
