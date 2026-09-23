#include "pdf/PdfSearchEngine.h"

namespace EtapKitap {

PdfSearchEngine::PdfSearchEngine(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PdfSearchEngine::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_results.size());
}

QVariant PdfSearchEngine::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_results.size())) {
        return QVariant();
    }

    const auto& res = m_results.at(index.row());
    switch (role) {
    case PageRole: return res.page;
    case TitleRole: return res.title;
    case SnippetRole: return res.snippet;
    default: return QVariant();
    }
}

QHash<int, QByteArray> PdfSearchEngine::roleNames() const {
    return {
        {PageRole, "page"},
        {TitleRole, "title"},
        {SnippetRole, "snippet"}
    };
}

int PdfSearchEngine::resultCount() const {
    return static_cast<int>(m_results.size());
}

bool PdfSearchEngine::isSearching() const {
    return m_isSearching;
}

void PdfSearchEngine::search(IDocument* doc, const QString& term) {
    clear();
    QString query = term.trimmed();
    if (!doc || query.isEmpty() || query.length() < 2) {
        return;
    }

    m_isSearching = true;
    emit isSearchingChanged(true);

    beginResetModel();
    const int total = doc->pageCount();
    for (int p = 1; p <= total; ++p) {
        QString text = doc->pageText(p);
        int idx = text.indexOf(query, 0, Qt::CaseInsensitive);
        if (idx >= 0) {
            int start = std::max(0, idx - 25);
            int len = std::min(static_cast<int>(text.length()) - start, static_cast<int>(query.length()) + 60);
            QString snippet = QStringLiteral("...") + text.mid(start, len).trimmed() + QStringLiteral("...");
            m_results.append(SearchResult{p, QStringLiteral("Sayfa %1").arg(p), snippet});
        }
    }
    endResetModel();

    m_isSearching = false;
    emit isSearchingChanged(false);
    emit resultCountChanged(resultCount());
}

void PdfSearchEngine::clear() {
    beginResetModel();
    m_results.clear();
    endResetModel();
    emit resultCountChanged(0);
}

} // namespace EtapKitap
