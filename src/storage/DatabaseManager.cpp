#include "storage/DatabaseManager.h"
#include "storage/Migrations.h"
#include "core/Logger.h"
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QUuid>
#include <QtSql/QSqlError>

namespace EtapKitap {

DatabaseManager::DatabaseManager(const QString& dbPath, QObject* parent)
    : QObject(parent)
    , m_dbPath(dbPath)
    , m_connectionName(QStringLiteral("etapkitap_") + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    if (m_dbPath.isEmpty()) {
        const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dataDir);
        m_dbPath = QDir(dataDir).filePath(QStringLiteral("library.db"));
    }
}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    QMutexLocker locker(&m_mutex);
    if (m_isOpen) {
        return true;
    }

    QDir().mkpath(QFileInfo(m_dbPath).dir().absolutePath());

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    db.setDatabaseName(m_dbPath);

    if (!db.open()) {
        qCCritical(logStorage) << "Failed to open SQLite database at" << m_dbPath << ":" << db.lastError().text();
        return false;
    }

    qCInfo(logStorage) << "Opened SQLite database at" << m_dbPath;

    if (!Migrations::apply(db)) {
        qCCritical(logStorage) << "Failed to apply database migrations.";
        db.close();
        return false;
    }

    m_isOpen = true;
    return true;
}

void DatabaseManager::close() {
    QMutexLocker locker(&m_mutex);
    if (!m_isOpen) {
        return;
    }

    {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName, false);
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(m_connectionName);
    m_isOpen = false;
    qCDebug(logStorage) << "Closed SQLite database.";
}

bool DatabaseManager::isOpen() const {
    QMutexLocker locker(&m_mutex);
    return m_isOpen;
}

QSqlDatabase DatabaseManager::database() const {
    return QSqlDatabase::database(m_connectionName);
}

QString DatabaseManager::databasePath() const {
    return m_dbPath;
}

bool DatabaseManager::transaction() {
    return database().transaction();
}

bool DatabaseManager::commit() {
    return database().commit();
}

bool DatabaseManager::rollback() {
    return database().rollback();
}

} // namespace EtapKitap
