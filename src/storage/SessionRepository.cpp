#include "storage/SessionRepository.h"
#include "core/Logger.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QUuid>
#include <QtCore/QDateTime>

namespace EtapKitap {

SessionRepository::SessionRepository(DatabaseManager* dbManager)
    : m_dbManager(dbManager)
{
}

bool SessionRepository::saveSession(const DocumentSession& session) {
    if (!m_dbManager || !m_dbManager->isOpen() || !session.isValid()) {
        return false;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral(
        "INSERT INTO document_sessions (document_id, current_page, zoom_level, view_mode, tool_dock_side, scroll_x, scroll_y, updated_at) "
        "VALUES (:id, :page, :zoom, :vmode, :dock, :sx, :sy, :upd) "
        "ON CONFLICT(document_id) DO UPDATE SET "
        "current_page = :page, zoom_level = :zoom, view_mode = :vmode, tool_dock_side = :dock, scroll_x = :sx, scroll_y = :sy, updated_at = :upd;"
    ));

    q.bindValue(QStringLiteral(":id"), session.documentId);
    q.bindValue(QStringLiteral(":page"), session.currentPage);
    q.bindValue(QStringLiteral(":zoom"), session.zoomLevel);
    q.bindValue(QStringLiteral(":vmode"), static_cast<int>(session.viewMode));
    q.bindValue(QStringLiteral(":dock"), session.toolDockSide);
    q.bindValue(QStringLiteral(":sx"), session.scrollX);
    q.bindValue(QStringLiteral(":sy"), session.scrollY);
    q.bindValue(QStringLiteral(":upd"), session.updatedAt.toString(Qt::ISODate));

    if (!q.exec()) {
        qCWarning(logStorage) << "Failed to save session for doc:" << session.documentId << ":" << q.lastError().text();
        return false;
    }
    return true;
}

std::optional<DocumentSession> SessionRepository::loadSession(const QString& documentId) {
    if (!m_dbManager || !m_dbManager->isOpen() || documentId.isEmpty()) {
        return std::nullopt;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("SELECT current_page, zoom_level, view_mode, tool_dock_side, scroll_x, scroll_y, updated_at "
                            "FROM document_sessions WHERE document_id = :id;"));
    q.bindValue(QStringLiteral(":id"), documentId);

    if (q.exec() && q.next()) {
        DocumentSession s;
        s.documentId = documentId;
        s.currentPage = q.value(0).toInt();
        s.zoomLevel = q.value(1).toDouble();
        s.viewMode = static_cast<ViewMode>(q.value(2).toInt());
        s.toolDockSide = q.value(3).toInt();
        s.scrollX = q.value(4).toDouble();
        s.scrollY = q.value(5).toDouble();
        s.updatedAt = QDateTime::fromString(q.value(6).toString(), Qt::ISODate);
        return s;
    }
    return std::nullopt;
}

bool SessionRepository::recordDocument(const QString& id, const QString& filePath, const QString& title, const QString& format, int pageCount) {
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return false;
    }

    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    QSqlQuery q(m_dbManager->database());

    q.prepare(QStringLiteral(
        "INSERT INTO documents (id, file_path, title, format, page_count, created_at, last_opened_at) "
        "VALUES (:id, :path, :title, :fmt, :cnt, :now, :now) "
        "ON CONFLICT(id) DO UPDATE SET last_opened_at = :now, page_count = :cnt, title = :title;"
    ));
    q.bindValue(QStringLiteral(":id"), id);
    q.bindValue(QStringLiteral(":path"), filePath);
    q.bindValue(QStringLiteral(":title"), title);
    q.bindValue(QStringLiteral(":fmt"), format);
    q.bindValue(QStringLiteral(":cnt"), pageCount);
    q.bindValue(QStringLiteral(":now"), now);

    if (!q.exec()) {
        qCWarning(logStorage) << "Failed to record document:" << q.lastError().text();
        return false;
    }

    // Also record in recent_documents
    QSqlQuery rq(m_dbManager->database());
    rq.prepare(QStringLiteral("INSERT INTO recent_documents (document_id, last_opened_at) VALUES (:id, :now) "
                             "ON CONFLICT(document_id) DO UPDATE SET last_opened_at = :now;"));
    rq.bindValue(QStringLiteral(":id"), id);
    rq.bindValue(QStringLiteral(":now"), now);
    rq.exec();

    return true;
}

QVector<RecentDocEntry> SessionRepository::getRecentDocuments(int limit) {
    QVector<RecentDocEntry> list;
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return list;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral(
        "SELECT d.id, d.file_path, d.title, d.format, d.page_count, COALESCE(s.current_page, 1), r.last_opened_at "
        "FROM recent_documents r "
        "JOIN documents d ON r.document_id = d.id "
        "LEFT JOIN document_sessions s ON d.id = s.document_id "
        "ORDER BY r.last_opened_at DESC LIMIT :lim;"
    ));
    q.bindValue(QStringLiteral(":lim"), limit);

    if (q.exec()) {
        while (q.next()) {
            RecentDocEntry entry;
            entry.id = q.value(0).toString();
            entry.filePath = q.value(1).toString();
            entry.title = q.value(2).toString();
            entry.format = q.value(3).toString();
            entry.pageCount = q.value(4).toInt();
            entry.lastPage = q.value(5).toInt();
            entry.lastOpenedAt = q.value(6).toString();
            list.append(entry);
        }
    }
    return list;
}

bool SessionRepository::addBookmark(const QString& documentId, int pageNumber, const QString& title) {
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return false;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("INSERT INTO bookmarks (id, document_id, page_number, title, created_at) "
                            "VALUES (:id, :docId, :page, :title, :now);"));
    q.bindValue(QStringLiteral(":id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    q.bindValue(QStringLiteral(":docId"), documentId);
    q.bindValue(QStringLiteral(":page"), pageNumber);
    q.bindValue(QStringLiteral(":title"), title.isEmpty() ? QStringLiteral("Sayfa %1").arg(pageNumber) : title);
    q.bindValue(QStringLiteral(":now"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate));

    return q.exec();
}

bool SessionRepository::removeBookmark(const QString& bookmarkId) {
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return false;
    }
    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("DELETE FROM bookmarks WHERE id = :id;"));
    q.bindValue(QStringLiteral(":id"), bookmarkId);
    return q.exec();
}

QVector<BookmarkEntry> SessionRepository::getBookmarks(const QString& documentId) {
    QVector<BookmarkEntry> list;
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return list;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("SELECT id, document_id, page_number, title FROM bookmarks WHERE document_id = :id ORDER BY page_number ASC;"));
    q.bindValue(QStringLiteral(":id"), documentId);

    if (q.exec()) {
        while (q.next()) {
            BookmarkEntry b;
            b.id = q.value(0).toString();
            b.documentId = q.value(1).toString();
            b.pageNumber = q.value(2).toInt();
            b.title = q.value(3).toString();
            list.append(b);
        }
    }
    return list;
}

} // namespace EtapKitap
