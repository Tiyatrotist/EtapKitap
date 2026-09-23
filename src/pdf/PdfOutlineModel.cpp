#include "pdf/PdfOutlineModel.h"

namespace EtapKitap {

PdfOutlineModel::PdfOutlineModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PdfOutlineModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_entries.size());
}

QVariant PdfOutlineModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_entries.size())) {
        return QVariant();
    }

    const auto& entry = m_entries.at(index.row());
    switch (role) {
    case TitleRole: return entry.title;
    case PageRole: return entry.page;
    case LevelRole: return entry.level;
    default: return QVariant();
    }
}

QHash<int, QByteArray> PdfOutlineModel::roleNames() const {
    return {
        {TitleRole, "title"},
        {PageRole, "page"},
        {LevelRole, "level"}
    };
}

void PdfOutlineModel::loadFromChapters(const QVector<ChapterInfo>& chapters) {
    beginResetModel();
    m_entries.clear();
    for (const auto& ch : chapters) {
        m_entries.append(OutlineEntry{ch.title, ch.page, 0});
    }
    endResetModel();
}

void PdfOutlineModel::clear() {
    beginResetModel();
    m_entries.clear();
    endResetModel();
}

} // namespace EtapKitap
