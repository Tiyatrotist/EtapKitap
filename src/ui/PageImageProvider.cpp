#include "ui/PageImageProvider.h"
#include <QtCore/QUrlQuery>
#include <QtCore/QUrl>

namespace EtapKitap {

PageImageProvider::PageImageProvider(DocumentController* controller)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_controller(controller)
{
}

QImage PageImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    if (!m_controller || !m_controller->hasDocument()) {
        return QImage();
    }

    // ID can be formatted as: "pageNumber" or "pageNumber?w=800&h=1100"
    int pageNumber = 1;
    int queryW = requestedSize.width();
    int queryH = requestedSize.height();

    int qIdx = id.indexOf('?');
    if (qIdx >= 0) {
        pageNumber = id.left(qIdx).toInt();
        QUrlQuery query(id.mid(qIdx + 1));
        if (query.hasQueryItem(QStringLiteral("w"))) {
            queryW = query.queryItemValue(QStringLiteral("w")).toInt();
        }
        if (query.hasQueryItem(QStringLiteral("h"))) {
            queryH = query.queryItemValue(QStringLiteral("h")).toInt();
        }
    } else {
        pageNumber = id.toInt();
    }

    if (pageNumber < 1) pageNumber = 1;

    QSize targetSize(queryW > 0 ? queryW : 800, queryH > 0 ? queryH : 1130);

    QImage img = m_controller->getCachedPageImage(pageNumber, targetSize);
    if (img.isNull()) {
        // Try any resolution in cache
        img = m_controller->getAnyResolutionImage(pageNumber);
        // Request fresh render in background
        m_controller->requestPageRender(pageNumber, targetSize);
    }

    if (img.isNull()) {
        // Fallback placeholder with same aspect ratio
        img = QImage(targetSize, QImage::Format_ARGB32_Premultiplied);
        img.fill(QColor(245, 245, 247));
    }

    if (size) {
        *size = img.size();
    }
    return img;
}

} // namespace EtapKitap
