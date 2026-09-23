#pragma once

#include "document/IDocument.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

namespace EtapKitap {

struct SearchResult {
    int page{1};
    QString title;
    QString snippet;
};

class PdfSearchEngine : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int resultCount READ resultCount NOTIFY resultCountChanged)
    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged)

public:
    enum Roles {
        PageRole = Qt::UserRole + 1,
        TitleRole,
        SnippetRole
    };

    explicit PdfSearchEngine(QObject* parent = nullptr);
    ~PdfSearchEngine() override = default;

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int resultCount() const;
    [[nodiscard]] bool isSearching() const;

    Q_INVOKABLE void search(IDocument* doc, const QString& term);
    Q_INVOKABLE void clear();

signals:
    void resultCountChanged(int count);
    void isSearchingChanged(bool searching);

private:
    QVector<SearchResult> m_results;
    bool m_isSearching{false};
};

} // namespace EtapKitap
