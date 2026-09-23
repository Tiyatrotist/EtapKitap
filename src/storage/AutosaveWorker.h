#pragma once

#include "document/DocumentSession.h"
#include "annotations/Annotation.h"
#include "storage/SessionRepository.h"
#include "storage/AnnotationRepository.h"
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QMutex>
#include <QtCore/QMap>
#include <memory>

namespace EtapKitap {

class AutosaveWorker : public QObject {
    Q_OBJECT

public:
    explicit AutosaveWorker(SessionRepository* sessionRepo, AnnotationRepository* annRepo, int debounceMs = 1000, QObject* parent = nullptr);
    ~AutosaveWorker() override;

    void triggerSessionSave(const DocumentSession& session);
    void triggerPageAnnotationsSave(const QString& documentId, int pageNumber, const QVector<std::shared_ptr<Annotation>>& annotations);
    void flushNow();

signals:
    void saveCompleted();

private slots:
    void onDebounceTimeout();

private:
    SessionRepository* m_sessionRepo;
    AnnotationRepository* m_annRepo;
    QTimer m_debounceTimer;
    QMutex m_mutex;

    bool m_hasPendingSession{false};
    DocumentSession m_pendingSession;

    struct PendingPageAnn {
        QString documentId;
        int pageNumber;
        QVector<std::shared_ptr<Annotation>> annotations;
    };
    QMap<QString, PendingPageAnn> m_pendingAnnotations; // key: docId_page
};

} // namespace EtapKitap
