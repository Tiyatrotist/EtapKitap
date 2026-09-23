#include "tools/FreezeCanvas.h"

namespace EtapKitap {

FreezeCanvas::FreezeCanvas(QObject* parent)
    : QObject(parent)
    , m_annotations(std::make_unique<AnnotationModel>())
{
}

bool FreezeCanvas::isActive() const { return m_active; }

void FreezeCanvas::setActive(bool active) {
    if (m_active != active) {
        m_active = active;
        if (!m_active) {
            clear();
        }
        emit activeChanged(m_active);
    }
}

AnnotationModel* FreezeCanvas::annotations() {
    return m_annotations.get();
}

const QImage& FreezeCanvas::snapshot() const {
    return m_snapshot;
}

void FreezeCanvas::setSnapshot(const QImage& snapshot) {
    m_snapshot = snapshot;
    emit snapshotChanged();
}

void FreezeCanvas::freeze(const QImage& snapshot) {
    m_snapshot = snapshot;
    m_annotations->clearAll();
    m_active = true;
    emit snapshotChanged();
    emit activeChanged(true);
}

void FreezeCanvas::unfreeze() {
    m_active = false;
    m_snapshot = QImage();
    m_annotations->clearAll();
    emit snapshotChanged();
    emit activeChanged(false);
}

void FreezeCanvas::clear() {
    m_annotations->clearAll();
}

} // namespace EtapKitap
