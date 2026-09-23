#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtGui/QColor>

namespace EtapKitap {

struct LaserPoint {
    QPointF pos;
    qint64 timestampMs{0};
};

class LaserPointer : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit LaserPointer(QObject* parent = nullptr);
    ~LaserPointer() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] QColor color() const;
    void setColor(const QColor& color);

    [[nodiscard]] qreal radius() const;
    void setRadius(qreal r);

    Q_INVOKABLE void addPoint(qreal x, qreal y);
    Q_INVOKABLE void clear();

    [[nodiscard]] const QVector<LaserPoint>& trailPoints() const;

signals:
    void activeChanged(bool active);
    void colorChanged(const QColor& color);
    void radiusChanged(qreal r);
    void trailUpdated();

private:
    void pruneExpiredPoints();

    bool m_active{false};
    QColor m_color{QColor(255, 40, 40)}; // Luminous red
    qreal m_radius{10.0};
    QVector<LaserPoint> m_points;
    QTimer m_fadeTimer;
    static constexpr qint64 TRAIL_LIFETIME_MS = 650; // smooth fade trail
};

} // namespace EtapKitap
