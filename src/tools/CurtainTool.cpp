#include "tools/CurtainTool.h"
#include <algorithm>

namespace EtapKitap {

CurtainTool::CurtainTool(QObject* parent)
    : QObject(parent)
{
}

bool CurtainTool::isActive() const { return m_active; }
void CurtainTool::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        emit activeChanged(m_active);
    }
}

CurtainDirection CurtainTool::direction() const { return m_direction; }
void CurtainTool::setDirection(CurtainDirection dir) {
    if (m_direction != dir) {
        m_direction = dir;
        emit directionChanged(m_direction);
    }
}

int CurtainTool::directionAsInt() const {
    return static_cast<int>(m_direction);
}

void CurtainTool::setDirectionAsInt(int dir) {
    setDirection(static_cast<CurtainDirection>(dir));
}

qreal CurtainTool::progress() const { return m_progress; }
void CurtainTool::setProgress(qreal p) {
    qreal clamped = std::clamp(p, 0.0, 1.0);
    if (!qFuzzyCompare(m_progress, clamped)) {
        m_progress = clamped;
        emit progressChanged(m_progress);
    }
}

qreal CurtainTool::opacity() const { return m_opacity; }
void CurtainTool::setOpacity(qreal op) {
    qreal clamped = std::clamp(op, 0.3, 1.0);
    if (!qFuzzyCompare(m_opacity, clamped)) {
        m_opacity = clamped;
        emit opacityChanged(m_opacity);
    }
}

void CurtainTool::reset() {
    setProgress(0.5);
}

void CurtainTool::openFull() {
    setProgress(0.0);
}

void CurtainTool::closeFull() {
    setProgress(1.0);
}

} // namespace EtapKitap
