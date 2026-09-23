#include "tools/WhiteboardEngine.h"

namespace EtapKitap {

WhiteboardEngine::WhiteboardEngine(QObject* parent)
    : QObject(parent)
{
    // Start with 1 page
    m_pages.push_back(std::make_unique<AnnotationModel>());
}

bool WhiteboardEngine::isActive() const { return m_active; }
void WhiteboardEngine::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        emit activeChanged(m_active);
    }
}

WhiteboardTemplate WhiteboardEngine::currentTemplate() const { return m_template; }
void WhiteboardEngine::setCurrentTemplate(WhiteboardTemplate templ) {
    if (m_template != templ) {
        m_template = templ;
        emit templateChanged(m_template);
    }
}

int WhiteboardEngine::currentTemplateAsInt() const {
    return static_cast<int>(m_template);
}

void WhiteboardEngine::setCurrentTemplateAsInt(int t) {
    setCurrentTemplate(static_cast<WhiteboardTemplate>(t));
}

int WhiteboardEngine::currentPage() const { return m_currentPage; }
void WhiteboardEngine::setCurrentPage(int p) {
    int clamped = std::clamp(p, 1, pageCount());
    if (m_currentPage != clamped) {
        m_currentPage = clamped;
        emit currentPageChanged(m_currentPage);
    }
}

int WhiteboardEngine::pageCount() const {
    return static_cast<int>(m_pages.size());
}

AnnotationModel* WhiteboardEngine::annotationsForPage(int page) {
    if (page >= 1 && page <= static_cast<int>(m_pages.size())) {
        return m_pages[page - 1].get();
    }
    return nullptr;
}

AnnotationModel* WhiteboardEngine::currentAnnotations() {
    return annotationsForPage(m_currentPage);
}

void WhiteboardEngine::addPage() {
    m_pages.push_back(std::make_unique<AnnotationModel>());
    emit pageCountChanged(pageCount());
    setCurrentPage(pageCount());
}

void WhiteboardEngine::nextPage() {
    if (m_currentPage < pageCount()) {
        setCurrentPage(m_currentPage + 1);
    }
}

void WhiteboardEngine::previousPage() {
    if (m_currentPage > 1) {
        setCurrentPage(m_currentPage - 1);
    }
}

void WhiteboardEngine::clearCurrentPage() {
    auto* ann = currentAnnotations();
    if (ann) {
        ann->clearAll();
    }
}

void WhiteboardEngine::renderBackground(QPainter& painter, qreal width, qreal height) const {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    switch (m_template) {
    case WhiteboardTemplate::BlankWhite:
        painter.fillRect(QRectF(0, 0, width, height), Qt::white);
        break;

    case WhiteboardTemplate::BlankBlack:
        painter.fillRect(QRectF(0, 0, width, height), QColor(28, 32, 38));
        break;

    case WhiteboardTemplate::Lined: {
        painter.fillRect(QRectF(0, 0, width, height), Qt::white);
        painter.setPen(QPen(QColor(220, 226, 235), 1.0));
        const qreal spacing = 40.0;
        for (qreal y = spacing * 1.5; y < height; y += spacing) {
            painter.drawLine(0, y, width, y);
        }
        // Red left margin line
        painter.setPen(QPen(QColor(240, 150, 150), 1.5));
        painter.drawLine(80.0, 0, 80.0, height);
        break;
    }

    case WhiteboardTemplate::Grid: {
        painter.fillRect(QRectF(0, 0, width, height), Qt::white);
        painter.setPen(QPen(QColor(225, 230, 238), 1.0));
        const qreal spacing = 35.0;
        for (qreal x = 0; x < width; x += spacing) {
            painter.drawLine(x, 0, x, height);
        }
        for (qreal y = 0; y < height; y += spacing) {
            painter.drawLine(0, y, width, y);
        }
        break;
    }

    case WhiteboardTemplate::Dotted: {
        painter.fillRect(QRectF(0, 0, width, height), Qt::white);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(180, 190, 205));
        const qreal spacing = 30.0;
        for (qreal x = spacing; x < width; x += spacing) {
            for (qreal y = spacing; y < height; y += spacing) {
                painter.drawEllipse(QPointF(x, y), 1.5, 1.5);
            }
        }
        break;
    }

    case WhiteboardTemplate::CoordinatePlane: {
        painter.fillRect(QRectF(0, 0, width, height), Qt::white);
        // Grid
        painter.setPen(QPen(QColor(230, 235, 242), 1.0));
        const qreal spacing = 30.0;
        const qreal cx = width / 2.0;
        const qreal cy = height / 2.0;

        for (qreal x = cx; x < width; x += spacing) painter.drawLine(x, 0, x, height);
        for (qreal x = cx; x > 0; x -= spacing) painter.drawLine(x, 0, x, height);
        for (qreal y = cy; y < height; y += spacing) painter.drawLine(0, y, width, y);
        for (qreal y = cy; y > 0; y -= spacing) painter.drawLine(0, y, width, y);

        // Major axes
        painter.setPen(QPen(QColor(70, 80, 95), 2.0));
        painter.drawLine(0, cy, width, cy);
        painter.drawLine(cx, 0, cx, height);
        break;
    }
    }

    painter.restore();
}

} // namespace EtapKitap
