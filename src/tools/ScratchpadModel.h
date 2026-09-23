#pragma once

#include "annotations/AnnotationModel.h"
#include <QtCore/QObject>
#include <memory>

namespace EtapKitap {

class ScratchpadModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int side READ side WRITE setSide NOTIFY sideChanged) // 0: Right, 1: Left, 2: Bottom
    Q_PROPERTY(qreal widthRatio READ widthRatio WRITE setWidthRatio NOTIFY widthRatioChanged)
    Q_PROPERTY(AnnotationModel* annotations READ annotations CONSTANT)

public:
    explicit ScratchpadModel(QObject* parent = nullptr);
    ~ScratchpadModel() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] int side() const;
    void setSide(int side);

    [[nodiscard]] qreal widthRatio() const;
    void setWidthRatio(qreal ratio);

    [[nodiscard]] AnnotationModel* annotations();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void toggle();

signals:
    void activeChanged(bool active);
    void sideChanged(int side);
    void widthRatioChanged(qreal ratio);

private:
    bool m_active{false};
    int m_side{0}; // Right pane default
    qreal m_widthRatio{0.35}; // 35% width
    std::unique_ptr<AnnotationModel> m_annotations;
};

} // namespace EtapKitap
