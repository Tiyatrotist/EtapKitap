#pragma once

#include "annotations/AnnotationModel.h"
#include "ui/viewmodels/ToolController.h"
#include <QtQuick/QQuickPaintedItem>

namespace EtapKitap {

class DrawingCanvasItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(int pageNumber READ pageNumber WRITE setPageNumber NOTIFY pageNumberChanged)
    Q_PROPERTY(AnnotationModel* annotationModel READ annotationModel WRITE setAnnotationModel NOTIFY annotationModelChanged)
    Q_PROPERTY(ToolController* toolController READ toolController WRITE setToolController NOTIFY toolControllerChanged)

public:
    explicit DrawingCanvasItem(QQuickItem* parent = nullptr);
    ~DrawingCanvasItem() override = default;

    [[nodiscard]] int pageNumber() const;
    void setPageNumber(int page);

    [[nodiscard]] AnnotationModel* annotationModel() const;
    void setAnnotationModel(AnnotationModel* model);

    [[nodiscard]] ToolController* toolController() const;
    void setToolController(ToolController* ctrl);

    void paint(QPainter* painter) override;

signals:
    void pageNumberChanged(int page);
    void annotationModelChanged();
    void toolControllerChanged();

private:
    int m_pageNumber{1};
    AnnotationModel* m_annotationModel{nullptr};
    ToolController* m_toolController{nullptr};
};

} // namespace EtapKitap
