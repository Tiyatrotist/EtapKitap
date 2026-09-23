#include "storage/AutosaveWorker.h"
#include "core/Logger.h"

namespace EtapKitap {

AutosaveWorker::AutosaveWorker(SessionRepository* sessionRepo, AnnotationRepository* annRepo, int debounceMs, QObject* parent)
    : QObject(parent)
    , m_sessionRepo(sessionRepo)
    , m_annRepo(annRepo)
{
    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setInterval(debounceMs);
    connect(&m_debounceTimer, &QTimer::timeout, this, &AutosaveWorker::onDebounceTimeout);
}

AutosaveWorker::~AutosaveWorker() {
    flushNow();
}

void AutosaveWorker::triggerSessionSave(const DocumentSession& session) {
    QMutexLocker locker(&m_mutex);
    m_pendingSession = session;
    m_hasPendingSession = true;

    QMetaObject::invokeMethod(&m_debounceTimer, qOverload<>(&QTimer::start), Qt::QueuedConnection);
}

void AutosaveWorker::triggerPageAnnotationsSave(const QString& documentId, int pageNumber, const QVector<std::shared_ptr<Annotation>>& annotations) {
    QMutexLocker locker(&m_mutex);
    QString key = QStringLiteral("%1_%2").arg(documentId).arg(pageNumber);
    m_pendingAnnotations[key] = PendingPageAnn{documentId, pageNumber, annotations};

    QMetaObject::invokeMethod(&m_debounceTimer, qOverload<>(&QTimer::start), Qt::QueuedConnection);
}

void AutosaveWorker::flushNow() {
    m_debounceTimer.stop();
    onDebounceTimeout();
}

void AutosaveWorker::onDebounceTimeout() {
    DocumentSession sessionToSave;
    bool hasSession = false;
    QMap<QString, PendingPageAnn> annsToSave;

    {
        QMutexLocker locker(&m_mutex);
        if (m_hasPendingSession) {
            sessionToSave = m_pendingSession;
            hasSession = true;
            m_hasPendingSession = false;
        }
        annsToSave = m_pendingAnnotations;
        m_pendingAnnotations.clear();
    }

    if (hasSession && m_sessionRepo) {
        m_sessionRepo->saveSession(sessionToSave);
        qCDebug(logStorage) << "Autosaved session for doc:" << sessionToSave.documentId << "page:" << sessionToSave.currentPage;
    }

    if (m_annRepo) {
        for (const auto& item : annsToSave) {
            m_annRepo->savePageAnnotations(item.documentId, item.pageNumber, item.annotations);
            qCDebug(logStorage) << "Autosaved" << item.annotations.size() << "annotations for doc:" << item.documentId << "page:" << item.pageNumber;
        }
    }

    emit saveCompleted();
}

} // namespace EtapKitap
