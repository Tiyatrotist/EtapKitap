#include "storage/Migrations.h"
#include "core/Logger.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QDateTime>

namespace EtapKitap {

bool Migrations::apply(QSqlDatabase& db) {
    if (!db.isOpen()) {
        qCCritical(logStorage) << "Cannot apply migrations on closed database.";
        return false;
    }

    QSqlQuery pragmaQuery(db);
    if (!pragmaQuery.exec(QStringLiteral("PRAGMA foreign_keys = ON;"))) {
        qCWarning(logStorage) << "Failed to enable foreign keys:" << pragmaQuery.lastError().text();
    }
    if (!pragmaQuery.exec(QStringLiteral("PRAGMA journal_mode = WAL;"))) {
        qCWarning(logStorage) << "Failed to enable WAL mode:" << pragmaQuery.lastError().text();
    }

    int currentVersion = getCurrentVersion(db);
    qCDebug(logStorage) << "Current database schema version:" << currentVersion;

    if (currentVersion < 1) {
        if (!applyVersion1(db)) {
            return false;
        }
    }

    return true;
}

int Migrations::getCurrentVersion(QSqlDatabase& db) {
    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS schema_version ("
                                  "version INTEGER NOT NULL,"
                                  "applied_at TEXT NOT NULL);"))) {
        qCCritical(logStorage) << "Failed to create schema_version table:" << query.lastError().text();
        return -1;
    }

    if (query.exec(QStringLiteral("SELECT MAX(version) FROM schema_version;")) && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool Migrations::applyVersion1(QSqlDatabase& db) {
    qCInfo(logStorage) << "Applying migration v1...";

    db.transaction();
    QSqlQuery q(db);

    const QStringList statements = {
        QStringLiteral("CREATE TABLE IF NOT EXISTS documents ("
                       "id TEXT PRIMARY KEY,"
                       "file_path TEXT UNIQUE NOT NULL,"
                       "title TEXT NOT NULL,"
                       "format TEXT NOT NULL,"
                       "page_count INTEGER NOT NULL DEFAULT 0,"
                       "created_at TEXT NOT NULL,"
                       "last_opened_at TEXT NOT NULL);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS document_sessions ("
                       "document_id TEXT PRIMARY KEY,"
                       "current_page INTEGER NOT NULL DEFAULT 1,"
                       "zoom_level REAL NOT NULL DEFAULT 1.0,"
                       "view_mode INTEGER NOT NULL DEFAULT 0,"
                       "tool_dock_side INTEGER NOT NULL DEFAULT 1,"
                       "scroll_x REAL NOT NULL DEFAULT 0.0,"
                       "scroll_y REAL NOT NULL DEFAULT 0.0,"
                       "updated_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS bookmarks ("
                       "id TEXT PRIMARY KEY,"
                       "document_id TEXT NOT NULL,"
                       "page_number INTEGER NOT NULL,"
                       "title TEXT NOT NULL,"
                       "created_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS annotations ("
                       "id TEXT PRIMARY KEY,"
                       "document_id TEXT NOT NULL,"
                       "page_number INTEGER NOT NULL,"
                       "tool_type INTEGER NOT NULL,"
                       "data_json TEXT NOT NULL,"
                       "created_at TEXT NOT NULL,"
                       "updated_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS canvas_pages ("
                       "id TEXT PRIMARY KEY,"
                       "document_id TEXT NOT NULL,"
                       "insert_after_page INTEGER NOT NULL,"
                       "background_template INTEGER NOT NULL DEFAULT 0,"
                       "title TEXT NOT NULL,"
                       "created_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS scratchpads ("
                       "id TEXT PRIMARY KEY,"
                       "document_id TEXT NOT NULL,"
                       "side INTEGER NOT NULL DEFAULT 0,"
                       "width_ratio REAL NOT NULL DEFAULT 0.35,"
                       "content_json TEXT NOT NULL,"
                       "updated_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS activity_state ("
                       "id TEXT PRIMARY KEY,"
                       "document_id TEXT NOT NULL,"
                       "activity_id TEXT NOT NULL,"
                       "page_number INTEGER NOT NULL,"
                       "state_json TEXT NOT NULL,"
                       "completed INTEGER NOT NULL DEFAULT 0,"
                       "score REAL NOT NULL DEFAULT 0.0,"
                       "updated_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS recent_documents ("
                       "document_id TEXT PRIMARY KEY,"
                       "last_opened_at TEXT NOT NULL,"
                       "FOREIGN KEY(document_id) REFERENCES documents(id) ON DELETE CASCADE);"),

        QStringLiteral("CREATE TABLE IF NOT EXISTS settings ("
                       "key TEXT PRIMARY KEY,"
                       "value TEXT NOT NULL);"),

        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_annotations_doc_page ON annotations(document_id, page_number);"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_bookmarks_doc ON bookmarks(document_id);"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_activity_doc ON activity_state(document_id);")
    };

    for (const auto& sql : statements) {
        if (!q.exec(sql)) {
            qCCritical(logStorage) << "Migration v1 failed executing:" << sql << "Error:" << q.lastError().text();
            db.rollback();
            return false;
        }
    }

    q.prepare(QStringLiteral("INSERT INTO schema_version (version, applied_at) VALUES (1, :appliedAt);"));
    q.bindValue(QStringLiteral(":appliedAt"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    if (!q.exec()) {
        qCCritical(logStorage) << "Failed to record schema_version v1:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    qCInfo(logStorage) << "Migration v1 applied successfully.";
    return true;
}

} // namespace EtapKitap
