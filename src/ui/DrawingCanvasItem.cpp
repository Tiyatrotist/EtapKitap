#include "ui/DrawingCanvasItem.h"

namespace EtapKitap {

DrawingCanvasItem::DrawingCanvasItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

int DrawingCanvasItem::pageNumber() const { return m_pageNumber; }
void DrawingCanvasItem::setPageNumber(int page) {
    if (m_pageNumber != page) {
        m_pageNumber = page;
        emit pageNumberChanged(m_pageNumber);
        update();
    }
}

AnnotationModel* DrawingCanvasItem::annotationModel() const { return m_annotationModel; }
void DrawingCanvasItem::setAnnotationModel(AnnotationModel* model) {
    if (m_annotationModel != model) {
        if (m_annotationModel) {
            disconnect(m_annotationModel, nullptr, this, nullptr);
        }
        m_annotationModel = model;
        if (m_annotationModel) {
            connect(m_annotationModel, &AnnotationModel::pageAnnotationsChanged, this, [this](int p) {
                if (p == m_pageNumber) update();
            });
            connect(m_annotationModel, &AnnotationModel::visibleChanged, this, [this]() {
                update();
            });
        }
        emit annotationModelChanged();
        update();
    }
}

ToolController* DrawingCanvasItem::toolController() const { return m_toolController; }
void DrawingCanvasItem::setToolController(ToolController* ctrl) {
    if (m_toolController != ctrl) {
        if (m_toolController) {
            disconnect(m_toolController, nullptr, this, nullptr);
        }
        m_toolController = ctrl;
        if (m_toolController) {
            connect(m_toolController, &ToolController::activeStrokeUpdated, this, [this]() {
                update();
            });
        }
        emit toolControllerChanged();
        update();
    }
}

void DrawingCanvasItem::paint(QPainter* painter) {
    if (!painter || width() <= 0 || height() <= 0) {
        return;
    }

    if (m_annotationModel) {
        m_annotationModel->renderPageAnnotations(*painter, m_pageNumber, width(), height());
    }
}

} // namespace EtapKitap
