#pragma once

#include <QtCore/QObject>
#include <QtCore/QRectF>

namespace EtapKitap {

class SpotlightTool : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool isCircle READ isCircle WRITE setCircle NOTIFY shapeChanged)
    Q_PROPERTY(qreal centerX READ centerX WRITE setCenterX NOTIFY positionChanged)
    Q_PROPERTY(qreal centerY READ centerY WRITE setCenterY NOTIFY positionChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY sizeChanged)
    Q_PROPERTY(qreal rectWidth READ rectWidth WRITE setRectWidth NOTIFY sizeChanged)
    Q_PROPERTY(qreal rectHeight READ rectHeight WRITE setRectHeight NOTIFY sizeChanged)
    Q_PROPERTY(qreal dimOpacity READ dimOpacity WRITE setDimOpacity NOTIFY opacityChanged)

public:
    explicit SpotlightTool(QObject* parent = nullptr);
    ~SpotlightTool() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] bool isCircle() const;
    void setCircle(bool circle);

    [[nodiscard]] qreal centerX() const;
    void setCenterX(qreal x);

    [[nodiscard]] qreal centerY() const;
    void setCenterY(qreal y);

    [[nodiscard]] qreal radius() const;
    void setRadius(qreal r);

    [[nodiscard]] qreal rectWidth() const;
    void setRectWidth(qreal w);

    [[nodiscard]] qreal rectHeight() const;
    void setRectHeight(qreal h);

    [[nodiscard]] qreal dimOpacity() const;
    void setDimOpacity(qreal op);

    Q_INVOKABLE void reset(qreal viewWidth, qreal viewHeight);
    Q_INVOKABLE void move(qreal dx, qreal dy);

signals:
    void activeChanged(bool active);
    void shapeChanged(bool isCircle);
    void positionChanged();
    void sizeChanged();
    void opacityChanged(qreal op);

private:
    bool m_active{false};
    bool m_isCircle{true};
    qreal m_centerX{400.0};
    qreal m_centerY{300.0};
    qreal m_radius{160.0};
    qreal m_rectWidth{320.0};
    qreal m_rectHeight{220.0};
    qreal m_dimOpacity{0.80};
};

} // namespace EtapKitap
