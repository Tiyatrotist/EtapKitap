#pragma once

#include "core/Types.h"
#include "annotations/AnnotationModel.h"
#include "annotations/StrokeAnnotation.h"
#include "annotations/ShapeAnnotation.h"
#include "annotations/UndoManager.h"
#include "tools/LaserPointer.h"
#include "tools/SpotlightTool.h"
#include "tools/CurtainTool.h"
#include <QtCore/QObject>
#include <QtGui/QColor>

namespace EtapKitap {

class ToolController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged)
    Q_PROPERTY(qreal penWidth READ penWidth WRITE setPenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(QColor highlighterColor READ highlighterColor WRITE setHighlighterColor NOTIFY highlighterColorChanged)
    Q_PROPERTY(qreal highlighterWidth READ highlighterWidth WRITE setHighlighterWidth NOTIFY highlighterWidthChanged)
    Q_PROPERTY(int eraserMode READ eraserModeAsInt WRITE setEraserModeAsInt NOTIFY eraserModeChanged)
    Q_PROPERTY(UndoManager* undoManager READ undoManager CONSTANT)
    Q_PROPERTY(LaserPointer* laserPointer READ laserPointer CONSTANT)
    Q_PROPERTY(SpotlightTool* spotlightTool READ spotlightTool CONSTANT)
    Q_PROPERTY(CurtainTool* curtainTool READ curtainTool CONSTANT)

public:
    explicit ToolController(AnnotationModel* annotationModel, UndoManager* undoManager, QObject* parent = nullptr);
    ~ToolController() override = default;

    [[nodiscard]] QColor penColor() const;
    void setPenColor(const QColor& c);

    [[nodiscard]] qreal penWidth() const;
    void setPenWidth(qreal w);

    [[nodiscard]] QColor highlighterColor() const;
    void setHighlighterColor(const QColor& c);

    [[nodiscard]] qreal highlighterWidth() const;
    void setHighlighterWidth(qreal w);

    [[nodiscard]] EraserMode eraserMode() const;
    void setEraserMode(EraserMode mode);
    [[nodiscard]] int eraserModeAsInt() const;
    void setEraserModeAsInt(int m);

    [[nodiscard]] UndoManager* undoManager();
    [[nodiscard]] LaserPointer* laserPointer();
    [[nodiscard]] SpotlightTool* spotlightTool();
    [[nodiscard]] CurtainTool* curtainTool();

    // Drawing hooks called from QML touch/mouse canvas
    Q_INVOKABLE void startStroke(int pageNumber, ToolType tool, qreal normX, qreal normY, qreal pressure = 1.0);
    Q_INVOKABLE void addStrokePoint(qreal normX, qreal normY, qreal pressure = 1.0);
    Q_INVOKABLE void endStroke();

    // Shape drawing hooks
    Q_INVOKABLE void startShape(int pageNumber, ToolType shapeTool, qreal startX, qreal startY);
    Q_INVOKABLE void updateShapeEnd(qreal curX, qreal curY);
    Q_INVOKABLE void endShape();

    // Eraser hook
    Q_INVOKABLE void eraseAt(int pageNumber, qreal normX, qreal normY, qreal radius = 0.02);

    // Page clear hook
    Q_INVOKABLE void clearPageAnnotations(int pageNumber);

signals:
    void penColorChanged(const QColor& c);
    void penWidthChanged(qreal w);
    void highlighterColorChanged(const QColor& c);
    void highlighterWidthChanged(qreal w);
    void eraserModeChanged(EraserMode mode);
    void activeStrokeUpdated();

private:
    AnnotationModel* m_annotationModel;
    UndoManager* m_undoManager;
    std::unique_ptr<LaserPointer> m_laser;
    std::unique_ptr<SpotlightTool> m_spotlight;
    std::unique_ptr<CurtainTool> m_curtain;

    QColor m_penColor{QColor(229, 57, 53)}; // #E53935
    qreal m_penWidth{0.004};
    QColor m_highlighterColor{QColor(255, 235, 59, 120)};
    qreal m_highlighterWidth{0.020};
    EraserMode m_eraserMode{EraserMode::Stroke};

    std::shared_ptr<StrokeAnnotation> m_activeStroke;
    std::shared_ptr<ShapeAnnotation> m_activeShape;
};

} // namespace EtapKitap
