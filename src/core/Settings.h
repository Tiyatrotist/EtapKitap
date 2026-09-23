#pragma once

#include "core/Types.h"
#include <QtCore/QObject>
#include <QtCore/QSettings>

namespace EtapKitap {

class Settings : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(qreal touchTargetScale READ touchTargetScale WRITE setTouchTargetScale NOTIFY touchTargetScaleChanged)
    Q_PROPERTY(bool leftHandedMode READ leftHandedMode WRITE setLeftHandedMode NOTIFY leftHandedModeChanged)
    Q_PROPERTY(bool smoothTransitions READ smoothTransitions WRITE setSmoothTransitions NOTIFY smoothTransitionsChanged)
    Q_PROPERTY(bool palmRejectionEnabled READ palmRejectionEnabled WRITE setPalmRejectionEnabled NOTIFY palmRejectionEnabledChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(int autosaveIntervalMs READ autosaveIntervalMs WRITE setAutosaveIntervalMs NOTIFY autosaveIntervalMsChanged)

public:
    explicit Settings(QObject* parent = nullptr);
    ~Settings() override = default;

    [[nodiscard]] QString theme() const;
    void setTheme(const QString& theme);

    [[nodiscard]] qreal touchTargetScale() const;
    void setTouchTargetScale(qreal scale);

    [[nodiscard]] bool leftHandedMode() const;
    void setLeftHandedMode(bool enabled);

    [[nodiscard]] bool smoothTransitions() const;
    void setSmoothTransitions(bool enabled);

    [[nodiscard]] bool palmRejectionEnabled() const;
    void setPalmRejectionEnabled(bool enabled);

    [[nodiscard]] QString language() const;
    void setLanguage(const QString& lang);

    [[nodiscard]] int autosaveIntervalMs() const;
    void setAutosaveIntervalMs(int ms);

    Q_INVOKABLE void sync();

signals:
    void themeChanged(const QString& theme);
    void touchTargetScaleChanged(qreal scale);
    void leftHandedModeChanged(bool enabled);
    void smoothTransitionsChanged(bool enabled);
    void palmRejectionEnabledChanged(bool enabled);
    void languageChanged(const QString& lang);
    void autosaveIntervalMsChanged(int ms);

private:
    QSettings m_settings;
    QString m_theme{"light"};
    qreal m_touchTargetScale{1.25}; // optimized for 75" touch boards
    bool m_leftHandedMode{false};
    bool m_smoothTransitions{true};
    bool m_palmRejectionEnabled{true};
    QString m_language{"tr"};
    int m_autosaveIntervalMs{1000};
};

} // namespace EtapKitap
