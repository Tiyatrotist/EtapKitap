#include "ui/WhiteboardCanvasItem.h"

namespace EtapKitap {

WhiteboardCanvasItem::WhiteboardCanvasItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

WhiteboardEngine* WhiteboardCanvasItem::engine() const { return m_engine; }
void WhiteboardCanvasItem::setEngine(WhiteboardEngine* eng) {
    if (m_engine != eng) {
        if (m_engine) {
            disconnect(m_engine, nullptr, this, nullptr);
        }
        m_engine = eng;
        if (m_engine) {
            connect(m_engine, &WhiteboardEngine::templateChanged, this, [this]() { update(); });
            connect(m_engine, &WhiteboardEngine::currentPageChanged, this, [this]() { update(); });
        }
        emit engineChanged();
        update();
    }
}

ToolController* WhiteboardCanvasItem::toolController() const { return m_toolController; }
void WhiteboardCanvasItem::setToolController(ToolController* ctrl) {
    if (m_toolController != ctrl) {
        if (m_toolController) {
            disconnect(m_toolController, nullptr, this, nullptr);
        }
        m_toolController = ctrl;
        if (m_toolController) {
            connect(m_toolController, &ToolController::activeStrokeUpdated, this, [this]() { update(); });
        }
        emit toolControllerChanged();
        update();
    }
}

void WhiteboardCanvasItem::paint(QPainter* painter) {
    if (!painter || !m_engine || width() <= 0 || height() <= 0) {
        return;
    }

    // 1. Render template background (grid, lined, etc.)
    m_engine->renderBackground(*painter, width(), height());

    // 2. Render whiteboard drawings for current page
    auto* annModel = m_engine->currentAnnotations();
    if (annModel) {
        annModel->renderPageAnnotations(*painter, 1, width(), height());
    }
}

} // namespace EtapKitap
