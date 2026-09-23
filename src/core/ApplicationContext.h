#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <memory>

namespace EtapKitap {

class Settings;
class DatabaseManager;

class ApplicationContext : public QObject {
    Q_OBJECT

public:
    static ApplicationContext& instance();

    void initialize();
    void shutdown();

    [[nodiscard]] Settings* settings() const;
    [[nodiscard]] DatabaseManager* databaseManager() const;

    [[nodiscard]] QString dataDirectory() const;
    [[nodiscard]] QString cacheDirectory() const;
    [[nodiscard]] QString thumbnailsDirectory() const;
    [[nodiscard]] QString sessionsDirectory() const;

private:
    ApplicationContext();
    ~ApplicationContext() override;

    ApplicationContext(const ApplicationContext&) = delete;
    ApplicationContext& operator=(const ApplicationContext&) = delete;

    std::unique_ptr<Settings> m_settings;
    std::unique_ptr<DatabaseManager> m_databaseManager;
    QString m_dataDir;
    QString m_cacheDir;
    QString m_thumbsDir;
    QString m_sessionsDir;
    bool m_initialized{false};
};

} // namespace EtapKitap
