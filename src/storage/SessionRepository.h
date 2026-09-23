#pragma once

#include "document/DocumentSession.h"
#include "storage/DatabaseManager.h"
#include <QtCore/QVector>
#include <optional>

namespace EtapKitap {

struct RecentDocEntry {
    QString id;
    QString filePath;
    QString title;
    QString format;
    int pageCount{0};
    int lastPage{1};
    QString lastOpenedAt;
};

struct BookmarkEntry {
    QString id;
    QString documentId;
    int pageNumber{1};
    QString title;
};

class SessionRepository {
public:
    explicit SessionRepository(DatabaseManager* dbManager);

    bool saveSession(const DocumentSession& session);
    [[nodiscard]] std::optional<DocumentSession> loadSession(const QString& documentId);

    bool recordDocument(const QString& id, const QString& filePath, const QString& title, const QString& format, int pageCount);
    [[nodiscard]] QVector<RecentDocEntry> getRecentDocuments(int limit = 10);

    bool addBookmark(const QString& documentId, int pageNumber, const QString& title);
    bool removeBookmark(const QString& bookmarkId);
    [[nodiscard]] QVector<BookmarkEntry> getBookmarks(const QString& documentId);

private:
    DatabaseManager* m_dbManager;
};

} // namespace EtapKitap
