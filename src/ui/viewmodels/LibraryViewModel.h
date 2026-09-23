#pragma once

#include "storage/SessionRepository.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

namespace EtapKitap {

class LibraryViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool hasRecentDocuments READ hasRecentDocuments NOTIFY recentDocumentsChanged)
    Q_PROPERTY(QString continueTitle READ continueTitle NOTIFY recentDocumentsChanged)
    Q_PROPERTY(int continuePage READ continuePage NOTIFY recentDocumentsChanged)
    Q_PROPERTY(QString continueFilePath READ continueFilePath NOTIFY recentDocumentsChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        FilePathRole,
        FormatRole,
        PageCountRole,
        LastPageRole,
        LastOpenedAtRole
    };

    explicit LibraryViewModel(SessionRepository* sessionRepo, QObject* parent = nullptr);
    ~LibraryViewModel() override = default;

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] bool hasRecentDocuments() const;
    [[nodiscard]] QString continueTitle() const;
    [[nodiscard]] int continuePage() const;
    [[nodiscard]] QString continueFilePath() const;

    Q_INVOKABLE void refreshRecents();
    [[nodiscard]] QString filePathAt(int index) const;

signals:
    void recentDocumentsChanged();

private:
    SessionRepository* m_sessionRepo;
    QVector<RecentDocEntry> m_recents;
};

} // namespace EtapKitap
