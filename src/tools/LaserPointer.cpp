#include "tools/LaserPointer.h"

namespace EtapKitap {

LaserPointer::LaserPointer(QObject* parent)
    : QObject(parent)
{
    m_fadeTimer.setInterval(25); // 40 FPS refresh for smooth fade
    connect(&m_fadeTimer, &QTimer::timeout, this, &LaserPointer::pruneExpiredPoints);
}

bool LaserPointer::isActive() const { return m_active; }

void LaserPointer::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        if (!m_active) {
            clear();
        }
        emit activeChanged(m_active);
    }
}

QColor LaserPointer::color() const { return m_color; }
void LaserPointer::setColor(const QColor& color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged(m_color);
    }
}

qreal LaserPointer::radius() const { return m_radius; }
void LaserPointer::setRadius(qreal r) {
    if (!qFuzzyCompare(m_radius, r)) {
        m_radius = r;
        emit radiusChanged(m_radius);
    }
}

void LaserPointer::addPoint(qreal x, qreal y) {
    if (!m_active) return;

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    m_points.append(LaserPoint{QPointF(x, y), now});

    if (!m_fadeTimer.isActive()) {
        m_fadeTimer.start();
    }
    emit trailUpdated();
}

void LaserPointer::clear() {
    m_points.clear();
    m_fadeTimer.stop();
    emit trailUpdated();
}

const QVector<LaserPoint>& LaserPointer::trailPoints() const {
    return m_points;
}

void LaserPointer::pruneExpiredPoints() {
    if (m_points.isEmpty()) {
        m_fadeTimer.stop();
        return;
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    int removeCount = 0;
    for (const auto& pt : m_points) {
        if (now - pt.timestampMs > TRAIL_LIFETIME_MS) {
            removeCount++;
        } else {
            break;
        }
    }

    if (removeCount > 0) {
        m_points.remove(0, removeCount);
    }

    if (m_points.isEmpty()) {
        m_fadeTimer.stop();
    }
    emit trailUpdated();
}

} // namespace EtapKitap
