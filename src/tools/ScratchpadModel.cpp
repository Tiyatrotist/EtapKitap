#include "tools/ScratchpadModel.h"
#include <algorithm>

namespace EtapKitap {

ScratchpadModel::ScratchpadModel(QObject* parent)
    : QObject(parent)
    , m_annotations(std::make_unique<AnnotationModel>())
{
}

bool ScratchpadModel::isActive() const { return m_active; }
void ScratchpadModel::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        emit activeChanged(m_active);
    }
}

int ScratchpadModel::side() const { return m_side; }
void ScratchpadModel::setSide(int side) {
    if (m_side != side) {
        m_side = side;
        emit sideChanged(m_side);
    }
}

qreal ScratchpadModel::widthRatio() const { return m_widthRatio; }
void ScratchpadModel::setWidthRatio(qreal ratio) {
    qreal clamped = std::clamp(ratio, 0.20, 0.70);
    if (!qFuzzyCompare(m_widthRatio, clamped)) {
        m_widthRatio = clamped;
        emit widthRatioChanged(m_widthRatio);
    }
}

AnnotationModel* ScratchpadModel::annotations() {
    return m_annotations.get();
}

void ScratchpadModel::clear() {
    m_annotations->clearAll();
}

void ScratchpadModel::toggle() {
    setActive(!m_active);
}

} // namespace EtapKitap
