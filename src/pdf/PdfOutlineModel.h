#pragma once

#include "book/BookManifest.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

namespace EtapKitap {

struct OutlineEntry {
    QString title;
    int page{1};
    int level{0};
};

class PdfOutlineModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        PageRole,
        LevelRole
    };

    explicit PdfOutlineModel(QObject* parent = nullptr);
    ~PdfOutlineModel() override = default;

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void loadFromChapters(const QVector<ChapterInfo>& chapters);
    void clear();

private:
    QVector<OutlineEntry> m_entries;
};

} // namespace EtapKitap
