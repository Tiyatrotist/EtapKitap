#pragma once

#include "annotations/Annotation.h"
#include <QtCore/QVector>

namespace EtapKitap {

class StrokeAnnotation : public Annotation {
public:
    StrokeAnnotation(int pageNumber, ToolType type = ToolType::Pen);
    ~StrokeAnnotation() override = default;

    [[nodiscard]] QString id() const override;
    [[nodiscard]] ToolType type() const override;
    [[nodiscard]] int pageNumber() const override;
    [[nodiscard]] NormalizedRect boundingRect() const override;
    [[nodiscard]] bool intersects(const NormalizedPoint& point, qreal tolerance) const override;

    void render(QPainter& painter, qreal pageWidth, qreal pageHeight) const override;
    [[nodiscard]] QJsonObject toJson() const override;
    static std::unique_ptr<StrokeAnnotation> fromJson(const QJsonObject& json);

    void addPoint(const NormalizedPoint& pt);
    [[nodiscard]] const QVector<NormalizedPoint>& points() const;
    void setPoints(const QVector<NormalizedPoint>& points);

    [[nodiscard]] QColor color() const;
    void setColor(const QColor& c);

    [[nodiscard]] qreal opacity() const;
    void setOpacity(qreal o);

    [[nodiscard]] qreal width() const;
    void setWidth(qreal w);

    void setId(const QString& id);

private:
    void updateBoundingRect();

    QString m_id;
    int m_pageNumber{1};
    ToolType m_type{ToolType::Pen};
    QVector<NormalizedPoint> m_points;
    QColor m_color{Qt::red};
    qreal m_opacity{1.0};
    qreal m_width{0.004}; // ~2-3 px on standard 600px width page
    NormalizedRect m_boundingRect;
};

} // namespace EtapKitap
