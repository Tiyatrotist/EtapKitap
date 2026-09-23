#pragma once

#include "annotations/Annotation.h"

namespace EtapKitap {

class ShapeAnnotation : public Annotation {
public:
    ShapeAnnotation(int pageNumber, ToolType shapeType);
    ~ShapeAnnotation() override = default;

    [[nodiscard]] QString id() const override;
    [[nodiscard]] ToolType type() const override;
    [[nodiscard]] int pageNumber() const override;
    [[nodiscard]] NormalizedRect boundingRect() const override;
    [[nodiscard]] bool intersects(const NormalizedPoint& point, qreal tolerance) const override;

    void render(QPainter& painter, qreal pageWidth, qreal pageHeight) const override;
    [[nodiscard]] QJsonObject toJson() const override;
    static std::unique_ptr<ShapeAnnotation> fromJson(const QJsonObject& json);

    void setStartPoint(const NormalizedPoint& pt);
    void setEndPoint(const NormalizedPoint& pt);
    [[nodiscard]] NormalizedPoint startPoint() const;
    [[nodiscard]] NormalizedPoint endPoint() const;

    [[nodiscard]] QColor color() const;
    void setColor(const QColor& c);

    [[nodiscard]] qreal width() const;
    void setWidth(qreal w);

    [[nodiscard]] bool isFilled() const;
    void setFilled(bool filled);
    [[nodiscard]] QColor fillColor() const;
    void setFillColor(const QColor& c);

    void setId(const QString& id);

private:
    QString m_id;
    int m_pageNumber{1};
    ToolType m_shapeType{ToolType::Rectangle};
    NormalizedPoint m_startPoint{0.0, 0.0};
    NormalizedPoint m_endPoint{0.0, 0.0};
    QColor m_color{Qt::red};
    qreal m_width{0.004};
    bool m_filled{false};
    QColor m_fillColor{Qt::transparent};
};

} // namespace EtapKitap
