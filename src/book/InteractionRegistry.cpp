#include "book/InteractionRegistry.h"
#include "core/Logger.h"

namespace EtapKitap {

InteractionRegistry::InteractionRegistry(QObject* parent)
    : QObject(parent)
{
}

void InteractionRegistry::loadFromManifest(const BookManifest& manifest) {
    clear();
    for (const auto& pi : manifest.pages) {
        m_pageInteractions[pi.pageNumber] = pi.interactions;
        for (const auto& hi : pi.interactions) {
            m_idMap[hi.id] = hi;
        }
    }
    qCDebug(logBook) << "Loaded" << m_idMap.size() << "interactions across" << m_pageInteractions.size() << "pages.";
    emit registryUpdated();
}

void InteractionRegistry::clear() {
    m_pageInteractions.clear();
    m_idMap.clear();
    emit registryUpdated();
}

QVector<HotspotInteraction> InteractionRegistry::interactionsForPage(int pageNumber) const {
    return m_pageInteractions.value(pageNumber);
}

int InteractionRegistry::interactionCountForPage(int pageNumber) const {
    return static_cast<int>(m_pageInteractions.value(pageNumber).size());
}

QMap<int, int> allPageInteractionCounts();
QMap<int, int> InteractionRegistry::allPageInteractionCounts() const {
    QMap<int, int> counts;
    for (auto it = m_pageInteractions.cbegin(); it != m_pageInteractions.cend(); ++it) {
        counts[it.key()] = static_cast<int>(it.value().size());
    }
    return counts;
}

const HotspotInteraction* InteractionRegistry::findAt(int pageNumber, const NormalizedPoint& pt) const {
    if (!m_visible || !m_pageInteractions.contains(pageNumber)) {
        return nullptr;
    }

    const auto& list = m_pageInteractions[pageNumber];
    for (const auto& hi : list) {
        if (hi.rect.contains(pt)) {
            return &hi;
        }
    }
    return nullptr;
}

const HotspotInteraction* InteractionRegistry::findById(const QString& id) const {
    auto it = m_idMap.find(id);
    if (it != m_idMap.end()) {
        return &(*it);
    }
    return nullptr;
}

bool InteractionRegistry::isVisible() const {
    return m_visible;
}

void InteractionRegistry::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        emit visibleChanged(m_visible);
    }
}

void InteractionRegistry::toggleVisible() {
    setVisible(!m_visible);
}

} // namespace EtapKitap
