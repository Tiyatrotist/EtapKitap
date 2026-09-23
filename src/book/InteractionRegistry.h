#pragma once

#include "book/BookManifest.h"
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVector>

namespace EtapKitap {

class InteractionRegistry : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)

public:
    explicit InteractionRegistry(QObject* parent = nullptr);
    ~InteractionRegistry() override = default;

    void loadFromManifest(const BookManifest& manifest);
    void clear();

    [[nodiscard]] QVector<HotspotInteraction> interactionsForPage(int pageNumber) const;
    [[nodiscard]] int interactionCountForPage(int pageNumber) const;
    [[nodiscard]] QMap<int, int> allPageInteractionCounts() const;

    [[nodiscard]] const HotspotInteraction* findAt(int pageNumber, const NormalizedPoint& pt) const;
    [[nodiscard]] const HotspotInteraction* findById(const QString& id) const;

    [[nodiscard]] bool isVisible() const;
    void setVisible(bool visible);
    Q_INVOKABLE void toggleVisible();

signals:
    void visibleChanged(bool visible);
    void registryUpdated();

private:
    QMap<int, QVector<HotspotInteraction>> m_pageInteractions;
    QMap<QString, HotspotInteraction> m_idMap;
    bool m_visible{true};
};

} // namespace EtapKitap
