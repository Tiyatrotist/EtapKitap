#pragma once

#include "document/IDocument.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

namespace EtapKitap {

struct PageInfo {
    int pageNumber{1};
    qreal width{595.0};
    qreal height{842.0};
    bool isCover{false};
    int interactionCount{0};
    bool hasAnnotations{false};
};

class PageModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum PageRoles {
        PageNumberRole = Qt::UserRole + 1,
        WidthRole,
        HeightRole,
        IsCoverRole,
        InteractionCountRole,
        HasAnnotationsRole
    };

    explicit PageModel(QObject* parent = nullptr);
    ~PageModel() override = default;

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setDocument(IDocument* doc, const QMap<int, int>& interactionCounts = {});
    void updateAnnotationsFlag(int pageNumber, bool hasAnnotations);
    void clear();

    [[nodiscard]] PageInfo pageInfo(int pageNumber) const;

private:
    QVector<PageInfo> m_pages;
};

} // namespace EtapKitap
