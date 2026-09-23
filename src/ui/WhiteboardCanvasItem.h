#pragma once

#include "tools/WhiteboardEngine.h"
#include "ui/viewmodels/ToolController.h"
#include <QtQuick/QQuickPaintedItem>

namespace EtapKitap {

class WhiteboardCanvasItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(WhiteboardEngine* engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(ToolController* toolController READ toolController WRITE setToolController NOTIFY toolControllerChanged)

public:
    explicit WhiteboardCanvasItem(QQuickItem* parent = nullptr);
    ~WhiteboardCanvasItem() override = default;

    [[nodiscard]] WhiteboardEngine* engine() const;
    void setEngine(WhiteboardEngine* eng);

    [[nodiscard]] ToolController* toolController() const;
    void setToolController(ToolController* ctrl);

    void paint(QPainter* painter) override;

signals:
    void engineChanged();
    void toolControllerChanged();

private:
    WhiteboardEngine* m_engine{nullptr};
    ToolController* m_toolController{nullptr};
};

} // namespace EtapKitap
