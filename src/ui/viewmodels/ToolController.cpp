#include "ui/viewmodels/ToolController.h"
#include "annotations/AnnotationCommands.h"

namespace EtapKitap {

ToolController::ToolController(AnnotationModel* annotationModel, UndoManager* undoManager, QObject* parent)
    : QObject(parent)
    , m_annotationModel(annotationModel)
    , m_undoManager(undoManager)
    , m_laser(std::make_unique<LaserPointer>())
    , m_spotlight(std::make_unique<SpotlightTool>())
    , m_curtain(std::make_unique<CurtainTool>())
{
}

QColor ToolController::penColor() const { return m_penColor; }
void ToolController::setPenColor(const QColor& c) {
    if (m_penColor != c) {
        m_penColor = c;
        emit penColorChanged(m_penColor);
    }
}

qreal ToolController::penWidth() const { return m_penWidth; }
void ToolController::setPenWidth(qreal w) {
    if (!qFuzzyCompare(m_penWidth, w)) {
        m_penWidth = w;
        emit penWidthChanged(m_penWidth);
    }
}

QColor ToolController::highlighterColor() const { return m_highlighterColor; }
void ToolController::setHighlighterColor(const QColor& c) {
    if (m_highlighterColor != c) {
        m_highlighterColor = c;
        emit highlighterColorChanged(m_highlighterColor);
    }
}

qreal ToolController::highlighterWidth() const { return m_highlighterWidth; }
void ToolController::setHighlighterWidth(qreal w) {
    if (!qFuzzyCompare(m_highlighterWidth, w)) {
        m_highlighterWidth = w;
        emit highlighterWidthChanged(m_highlighterWidth);
    }
}

EraserMode ToolController::eraserMode() const { return m_eraserMode; }
void ToolController::setEraserMode(EraserMode mode) {
    if (m_eraserMode != mode) {
        m_eraserMode = mode;
        emit eraserModeChanged(m_eraserMode);
    }
}

int ToolController::eraserModeAsInt() const {
    return static_cast<int>(m_eraserMode);
}
void ToolController::setEraserModeAsInt(int m) {
    setEraserMode(static_cast<EraserMode>(m));
}

UndoManager* ToolController::undoManager() { return m_undoManager; }
LaserPointer* ToolController::laserPointer() { return m_laser.get(); }
SpotlightTool* ToolController::spotlightTool() { return m_spotlight.get(); }
CurtainTool* ToolController::curtainTool() { return m_curtain.get(); }

void ToolController::startStroke(int pageNumber, ToolType tool, qreal normX, qreal normY, qreal pressure) {
    m_activeStroke = std::make_shared<StrokeAnnotation>(pageNumber, tool);
    if (tool == ToolType::Highlighter) {
        m_activeStroke->setColor(m_highlighterColor);
        m_activeStroke->setWidth(m_highlighterWidth);
    } else {
        m_activeStroke->setColor(m_penColor);
        m_activeStroke->setWidth(m_penWidth);
    }
    m_activeStroke->addPoint(NormalizedPoint{normX, normY, pressure});
    emit activeStrokeUpdated();
}

void ToolController::addStrokePoint(qreal normX, qreal normY, qreal pressure) {
    if (m_activeStroke) {
        m_activeStroke->addPoint(NormalizedPoint{normX, normY, pressure});
        emit activeStrokeUpdated();
    }
}

void ToolController::endStroke() {
    if (m_activeStroke && m_annotationModel) {
        if (m_activeStroke->points().size() >= 2) {
            auto cmd = new AddAnnotationCommand(m_annotationModel, m_activeStroke);
            if (m_undoManager) {
                m_undoManager->push(cmd);
            } else {
                m_annotationModel->addAnnotation(m_activeStroke);
            }
        }
        m_activeStroke.reset();
        emit activeStrokeUpdated();
    }
}

void ToolController::startShape(int pageNumber, ToolType shapeTool, qreal startX, qreal startY) {
    m_activeShape = std::make_shared<ShapeAnnotation>(pageNumber, shapeTool);
    m_activeShape->setColor(m_penColor);
    m_activeShape->setWidth(m_penWidth);
    m_activeShape->setStartPoint(NormalizedPoint{startX, startY});
    m_activeShape->setEndPoint(NormalizedPoint{startX, startY});
    emit activeStrokeUpdated();
}

void ToolController::updateShapeEnd(qreal curX, qreal curY) {
    if (m_activeShape) {
        m_activeShape->setEndPoint(NormalizedPoint{curX, curY});
        emit activeStrokeUpdated();
    }
}

void ToolController::endShape() {
    if (m_activeShape && m_annotationModel) {
        auto cmd = new AddAnnotationCommand(m_annotationModel, m_activeShape);
        if (m_undoManager) {
            m_undoManager->push(cmd);
        } else {
            m_annotationModel->addAnnotation(m_activeShape);
        }
        m_activeShape.reset();
        emit activeStrokeUpdated();
    }
}

void ToolController::eraseAt(int pageNumber, qreal normX, qreal normY, qreal radius) {
    if (!m_annotationModel) return;

    NormalizedPoint pt{normX, normY};
    auto hit = m_annotationModel->findIntersecting(pageNumber, pt, radius);
    if (hit) {
        auto cmd = new RemoveAnnotationCommand(m_annotationModel, hit);
        if (m_undoManager) {
            m_undoManager->push(cmd);
        } else {
            m_annotationModel->removeAnnotation(hit->id());
        }
    }
}

void ToolController::clearPageAnnotations(int pageNumber) {
    if (!m_annotationModel) return;
    auto cmd = new ClearPageAnnotationsCommand(m_annotationModel, pageNumber);
    if (m_undoManager) {
        m_undoManager->push(cmd);
    } else {
        m_annotationModel->clearPage(pageNumber);
    }
}

} // namespace EtapKitap
