#pragma once

#include "core/Types.h"
#include <QtCore/QObject>

namespace EtapKitap {

class CurtainTool : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int direction READ directionAsInt WRITE setDirectionAsInt NOTIFY directionChanged)
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

public:
    explicit CurtainTool(QObject* parent = nullptr);
    ~CurtainTool() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] CurtainDirection direction() const;
    void setDirection(CurtainDirection dir);
    [[nodiscard]] int directionAsInt() const;
    void setDirectionAsInt(int dir);

    [[nodiscard]] qreal progress() const;
    void setProgress(qreal p);

    [[nodiscard]] qreal opacity() const;
    void setOpacity(qreal op);

    Q_INVOKABLE void reset();
    Q_INVOKABLE void openFull();
    Q_INVOKABLE void closeFull();

signals:
    void activeChanged(bool active);
    void directionChanged(CurtainDirection dir);
    void progressChanged(qreal progress);
    void opacityChanged(qreal opacity);

private:
    bool m_active{false};
    CurtainDirection m_direction{CurtainDirection::TopToBottom};
    qreal m_progress{0.5}; // 50% covering
    qreal m_opacity{0.95};
};

} // namespace EtapKitap
