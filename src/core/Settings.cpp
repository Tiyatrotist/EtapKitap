#include "core/Settings.h"
#include "core/Logger.h"

namespace EtapKitap {

Settings::Settings(QObject* parent)
    : QObject(parent)
    , m_settings(QStringLiteral("Tiyatrotist"), QStringLiteral("EtapKitap"))
{
    m_theme = m_settings.value(QStringLiteral("appearance/theme"), QStringLiteral("light")).toString();
    m_touchTargetScale = m_settings.value(QStringLiteral("accessibility/touchScale"), 1.25).toDouble();
    m_leftHandedMode = m_settings.value(QStringLiteral("accessibility/leftHanded"), false).toBool();
    m_smoothTransitions = m_settings.value(QStringLiteral("performance/smoothTransitions"), true).toBool();
    m_palmRejectionEnabled = m_settings.value(QStringLiteral("input/palmRejection"), true).toBool();
    m_language = m_settings.value(QStringLiteral("locale/language"), QStringLiteral("tr")).toString();
    m_autosaveIntervalMs = m_settings.value(QStringLiteral("storage/autosaveMs"), 1000).toInt();

    qCDebug(logCore) << "Settings loaded. Theme:" << m_theme << "Scale:" << m_touchTargetScale << "LeftHanded:" << m_leftHandedMode;
}

QString Settings::theme() const { return m_theme; }
void Settings::setTheme(const QString& theme) {
    if (m_theme != theme) {
        m_theme = theme;
        m_settings.setValue(QStringLiteral("appearance/theme"), m_theme);
        emit themeChanged(m_theme);
    }
}

qreal Settings::touchTargetScale() const { return m_touchTargetScale; }
void Settings::setTouchTargetScale(qreal scale) {
    if (!qFuzzyCompare(m_touchTargetScale, scale)) {
        m_touchTargetScale = scale;
        m_settings.setValue(QStringLiteral("accessibility/touchScale"), m_touchTargetScale);
        emit touchTargetScaleChanged(m_touchTargetScale);
    }
}

bool Settings::leftHandedMode() const { return m_leftHandedMode; }
void Settings::setLeftHandedMode(bool enabled) {
    if (m_leftHandedMode != enabled) {
        m_leftHandedMode = enabled;
        m_settings.setValue(QStringLiteral("accessibility/leftHanded"), m_leftHandedMode);
        emit leftHandedModeChanged(m_leftHandedMode);
    }
}

bool Settings::smoothTransitions() const { return m_smoothTransitions; }
void Settings::setSmoothTransitions(bool enabled) {
    if (m_smoothTransitions != enabled) {
        m_smoothTransitions = enabled;
        m_settings.setValue(QStringLiteral("performance/smoothTransitions"), m_smoothTransitions);
        emit smoothTransitionsChanged(m_smoothTransitions);
    }
}

bool Settings::palmRejectionEnabled() const { return m_palmRejectionEnabled; }
void Settings::setPalmRejectionEnabled(bool enabled) {
    if (m_palmRejectionEnabled != enabled) {
        m_palmRejectionEnabled = enabled;
        m_settings.setValue(QStringLiteral("input/palmRejection"), m_palmRejectionEnabled);
        emit palmRejectionEnabledChanged(m_palmRejectionEnabled);
    }
}

QString Settings::language() const { return m_language; }
void Settings::setLanguage(const QString& lang) {
    if (m_language != lang) {
        m_language = lang;
        m_settings.setValue(QStringLiteral("locale/language"), m_language);
        emit languageChanged(m_language);
    }
}

int Settings::autosaveIntervalMs() const { return m_autosaveIntervalMs; }
void Settings::setAutosaveIntervalMs(int ms) {
    if (m_autosaveIntervalMs != ms) {
        m_autosaveIntervalMs = ms;
        m_settings.setValue(QStringLiteral("storage/autosaveMs"), m_autosaveIntervalMs);
        emit autosaveIntervalMsChanged(m_autosaveIntervalMs);
    }
}

void Settings::sync() {
    m_settings.sync();
}

} // namespace EtapKitap
