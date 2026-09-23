#include "document/PageModel.h"
#include "core/Logger.h"

namespace EtapKitap {

PageModel::PageModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PageModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_pages.size());
}

QVariant PageModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_pages.size())) {
        return QVariant();
    }

    const auto& page = m_pages.at(index.row());
    switch (role) {
    case PageNumberRole:
        return page.pageNumber;
    case WidthRole:
        return page.width;
    case HeightRole:
        return page.height;
    case IsCoverRole:
        return page.isCover;
    case InteractionCountRole:
        return page.interactionCount;
    case HasAnnotationsRole:
        return page.hasAnnotations;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PageModel::roleNames() const {
    return {
        {PageNumberRole, "pageNumber"},
        {WidthRole, "width"},
        {HeightRole, "height"},
        {IsCoverRole, "isCover"},
        {InteractionCountRole, "interactionCount"},
        {HasAnnotationsRole, "hasAnnotations"}
    };
}

void PageModel::setDocument(IDocument* doc, const QMap<int, int>& interactionCounts) {
    beginResetModel();
    m_pages.clear();
    if (doc) {
        const int count = doc->pageCount();
        m_pages.reserve(count);
        for (int i = 1; i <= count; ++i) {
            QSizeF sz = doc->pageSize(i);
            PageInfo info;
            info.pageNumber = i;
            info.width = sz.width();
            info.height = sz.height();
            info.isCover = (i == 1);
            info.interactionCount = interactionCounts.value(i, 0);
            info.hasAnnotations = false;
            m_pages.append(info);
        }
    }
    endResetModel();
    qCDebug(logCore) << "PageModel populated with" << m_pages.size() << "pages.";
}

void PageModel::updateAnnotationsFlag(int pageNumber, bool hasAnnotations) {
    if (pageNumber >= 1 && pageNumber <= static_cast<int>(m_pages.size())) {
        m_pages[pageNumber - 1].hasAnnotations = hasAnnotations;
        QModelIndex idx = index(pageNumber - 1, 0);
        emit dataChanged(idx, idx, {HasAnnotationsRole});
    }
}

void PageModel::clear() {
    beginResetModel();
    m_pages.clear();
    endResetModel();
}

PageInfo PageModel::pageInfo(int pageNumber) const {
    if (pageNumber >= 1 && pageNumber <= static_cast<int>(m_pages.size())) {
        return m_pages.at(pageNumber - 1);
    }
    return PageInfo{};
}

} // namespace EtapKitap
