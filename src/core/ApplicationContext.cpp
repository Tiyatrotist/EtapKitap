#include "core/ApplicationContext.h"
#include "core/Settings.h"
#include "storage/DatabaseManager.h"
#include "core/Logger.h"
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

namespace EtapKitap {

ApplicationContext& ApplicationContext::instance() {
    static ApplicationContext ctx;
    return ctx;
}

ApplicationContext::ApplicationContext() = default;
ApplicationContext::~ApplicationContext() {
    shutdown();
}

void ApplicationContext::initialize() {
    if (m_initialized) {
        return;
    }

    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_dataDir = appData;
    m_cacheDir = QDir(appData).filePath(QStringLiteral("cache"));
    m_thumbsDir = QDir(appData).filePath(QStringLiteral("thumbnails"));
    m_sessionsDir = QDir(appData).filePath(QStringLiteral("sessions"));

    QDir().mkpath(m_dataDir);
    QDir().mkpath(m_cacheDir);
    QDir().mkpath(m_thumbsDir);
    QDir().mkpath(m_sessionsDir);

    m_settings = std::make_unique<Settings>();
    m_databaseManager = std::make_unique<DatabaseManager>(QDir(m_dataDir).filePath(QStringLiteral("library.db")));
    m_databaseManager->open();

    m_initialized = true;
    qCInfo(logCore) << "ApplicationContext initialized. Data dir:" << m_dataDir;
}

void ApplicationContext::shutdown() {
    if (!m_initialized) {
        return;
    }
    if (m_databaseManager) {
        m_databaseManager->close();
    }
    m_initialized = false;
    qCInfo(logCore) << "ApplicationContext shut down.";
}

Settings* ApplicationContext::settings() const {
    return m_settings.get();
}

DatabaseManager* ApplicationContext::databaseManager() const {
    return m_databaseManager.get();
}

QString ApplicationContext::dataDirectory() const { return m_dataDir; }
QString ApplicationContext::cacheDirectory() const { return m_cacheDir; }
QString ApplicationContext::thumbnailsDirectory() const { return m_thumbsDir; }
QString ApplicationContext::sessionsDirectory() const { return m_sessionsDir; }

} // namespace EtapKitap
