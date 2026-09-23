#include "tools/SpotlightTool.h"
#include <algorithm>

namespace EtapKitap {

SpotlightTool::SpotlightTool(QObject* parent)
    : QObject(parent)
{
}

bool SpotlightTool::isActive() const { return m_active; }
void SpotlightTool::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        emit activeChanged(m_active);
    }
}

bool SpotlightTool::isCircle() const { return m_isCircle; }
void SpotlightTool::setCircle(bool circle) {
    if (m_isCircle != circle) {
        m_isCircle = circle;
        emit shapeChanged(m_isCircle);
    }
}

qreal SpotlightTool::centerX() const { return m_centerX; }
void SpotlightTool::setCenterX(qreal x) {
    if (!qFuzzyCompare(m_centerX, x)) {
        m_centerX = x;
        emit positionChanged();
    }
}

qreal SpotlightTool::centerY() const { return m_centerY; }
void SpotlightTool::setCenterY(qreal y) {
    if (!qFuzzyCompare(m_centerY, y)) {
        m_centerY = y;
        emit positionChanged();
    }
}

qreal SpotlightTool::radius() const { return m_radius; }
void SpotlightTool::setRadius(qreal r) {
    qreal clamped = std::clamp(r, 40.0, 800.0);
    if (!qFuzzyCompare(m_radius, clamped)) {
        m_radius = clamped;
        emit sizeChanged();
    }
}

qreal SpotlightTool::rectWidth() const { return m_rectWidth; }
void SpotlightTool::setRectWidth(qreal w) {
    qreal clamped = std::clamp(w, 80.0, 1600.0);
    if (!qFuzzyCompare(m_rectWidth, clamped)) {
        m_rectWidth = clamped;
        emit sizeChanged();
    }
}

qreal SpotlightTool::rectHeight() const { return m_rectHeight; }
void SpotlightTool::setRectHeight(qreal h) {
    qreal clamped = std::clamp(h, 60.0, 1200.0);
    if (!qFuzzyCompare(m_rectHeight, clamped)) {
        m_rectHeight = clamped;
        emit sizeChanged();
    }
}

qreal SpotlightTool::dimOpacity() const { return m_dimOpacity; }
void SpotlightTool::setDimOpacity(qreal op) {
    qreal clamped = std::clamp(op, 0.2, 0.98);
    if (!qFuzzyCompare(m_dimOpacity, clamped)) {
        m_dimOpacity = clamped;
        emit opacityChanged(m_dimOpacity);
    }
}

void SpotlightTool::reset(qreal viewWidth, qreal viewHeight) {
    m_centerX = viewWidth > 0 ? (viewWidth / 2.0) : 400.0;
    m_centerY = viewHeight > 0 ? (viewHeight / 2.0) : 300.0;
    m_radius = 160.0;
    m_rectWidth = 320.0;
    m_rectHeight = 220.0;
    emit positionChanged();
    emit sizeChanged();
}

void SpotlightTool::move(qreal dx, qreal dy) {
    m_centerX += dx;
    m_centerY += dy;
    emit positionChanged();
}

} // namespace EtapKitap
