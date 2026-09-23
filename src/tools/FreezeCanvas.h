#pragma once

#include "annotations/AnnotationModel.h"
#include <QtCore/QObject>
#include <QtGui/QImage>

namespace EtapKitap {

class FreezeCanvas : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(AnnotationModel* annotations READ annotations CONSTANT)

public:
    explicit FreezeCanvas(QObject* parent = nullptr);
    ~FreezeCanvas() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] AnnotationModel* annotations();
    [[nodiscard]] const QImage& snapshot() const;
    void setSnapshot(const QImage& snapshot);

    Q_INVOKABLE void freeze(const QImage& snapshot);
    Q_INVOKABLE void unfreeze();
    Q_INVOKABLE void clear();

signals:
    void activeChanged(bool active);
    void snapshotChanged();

private:
    bool m_active{false};
    QImage m_snapshot;
    std::unique_ptr<AnnotationModel> m_annotations;
};

} // namespace EtapKitap
