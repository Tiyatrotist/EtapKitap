#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtSql/QSqlDatabase>
#include <memory>

namespace EtapKitap {

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    explicit DatabaseManager(const QString& dbPath = QString(), QObject* parent = nullptr);
    ~DatabaseManager() override;

    bool open();
    void close();
    [[nodiscard]] bool isOpen() const;

    [[nodiscard]] QSqlDatabase database() const;
    [[nodiscard]] QString databasePath() const;

    // Direct transaction helpers
    bool transaction();
    bool commit();
    bool rollback();

private:
    QString m_dbPath;
    QString m_connectionName;
    mutable QMutex m_mutex;
    bool m_isOpen{false};
};

} // namespace EtapKitap
