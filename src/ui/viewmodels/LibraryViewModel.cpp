#include "ui/viewmodels/LibraryViewModel.h"

namespace EtapKitap {

LibraryViewModel::LibraryViewModel(SessionRepository* sessionRepo, QObject* parent)
    : QAbstractListModel(parent)
    , m_sessionRepo(sessionRepo)
{
    refreshRecents();
}

int LibraryViewModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_recents.size());
}

QVariant LibraryViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_recents.size())) {
        return QVariant();
    }

    const auto& item = m_recents.at(index.row());
    switch (role) {
    case IdRole: return item.id;
    case TitleRole: return item.title;
    case FilePathRole: return item.filePath;
    case FormatRole: return item.format;
    case PageCountRole: return item.pageCount;
    case LastPageRole: return item.lastPage;
    case LastOpenedAtRole: return item.lastOpenedAt;
    default: return QVariant();
    }
}

QHash<int, QByteArray> LibraryViewModel::roleNames() const {
    return {
        {IdRole, "id"},
        {TitleRole, "title"},
        {FilePathRole, "filePath"},
        {FormatRole, "format"},
        {PageCountRole, "pageCount"},
        {LastPageRole, "lastPage"},
        {LastOpenedAtRole, "lastOpenedAt"}
    };
}

bool LibraryViewModel::hasRecentDocuments() const {
    return !m_recents.isEmpty();
}

QString LibraryViewModel::continueTitle() const {
    return m_recents.isEmpty() ? QString() : m_recents.first().title;
}

int LibraryViewModel::continuePage() const {
    return m_recents.isEmpty() ? 1 : m_recents.first().lastPage;
}

QString LibraryViewModel::continueFilePath() const {
    return m_recents.isEmpty() ? QString() : m_recents.first().filePath;
}

void LibraryViewModel::refreshRecents() {
    beginResetModel();
    if (m_sessionRepo) {
        m_recents = m_sessionRepo->getRecentDocuments(12);
    } else {
        m_recents.clear();
    }
    endResetModel();
    emit recentDocumentsChanged();
}

QString LibraryViewModel::filePathAt(int index) const {
    if (index >= 0 && index < m_recents.size()) {
        return m_recents.at(index).filePath;
    }
    return QString();
}

} // namespace EtapKitap
