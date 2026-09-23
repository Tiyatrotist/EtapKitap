#include "pdf/PdfRenderQueue.h"
#include "core/Logger.h"
#include <QtCore/QRunnable>

namespace EtapKitap {

class RenderTask : public QRunnable {
public:
    RenderTask(PdfRenderQueue* queue, IPdfBackend* backend, PdfPageCache* cache,
               int pageNumber, const QSize& targetSize, const QString& reqKey)
        : m_queue(queue)
        , m_backend(backend)
        , m_cache(cache)
        , m_pageNumber(pageNumber)
        , m_targetSize(targetSize)
        , m_reqKey(reqKey)
    {
        setAutoDelete(true);
    }

    void run() override {
        if (!m_backend || !m_backend->isLoaded()) {
            return;
        }

        QImage img = m_backend->renderPage(m_pageNumber, m_targetSize);
        if (!img.isNull() && m_cache) {
            m_cache->insert(m_pageNumber, m_targetSize, img);
        }

        QMetaObject::invokeMethod(m_queue, [queue = m_queue, p = m_pageNumber, s = m_targetSize, img, key = m_reqKey]() {
            emit queue->pageRendered(p, s, img);
        }, Qt::QueuedConnection);
    }

private:
    PdfRenderQueue* m_queue;
    IPdfBackend* m_backend;
    PdfPageCache* m_cache;
    int m_pageNumber;
    QSize m_targetSize;
    QString m_reqKey;
};

PdfRenderQueue::PdfRenderQueue(IPdfBackend* backend, PdfPageCache* cache, QObject* parent)
    : QObject(parent)
    , m_backend(backend)
    , m_cache(cache)
{
    m_threadPool.setMaxThreadCount(qMax(2, QThread::idealThreadCount() / 2));
}

PdfRenderQueue::~PdfRenderQueue() {
    cancelAll();
}

void PdfRenderQueue::requestPage(int pageNumber, const QSize& targetSize, int priority) {
    if (!m_backend || !m_backend->isLoaded() || pageNumber < 1 || targetSize.isEmpty()) {
        return;
    }

    // If cache already has it, emit immediately
    if (m_cache && m_cache->contains(pageNumber, targetSize)) {
        QImage cachedImg = m_cache->get(pageNumber, targetSize);
        emit pageRendered(pageNumber, targetSize, cachedImg);
        return;
    }

    const QString reqKey = QStringLiteral("%1_%2x%3").arg(pageNumber).arg(targetSize.width()).arg(targetSize.height());

    {
        QMutexLocker locker(&m_mutex);
        if (m_activeRequests.contains(reqKey)) {
            return;
        }
        m_activeRequests.insert(reqKey);
    }

    auto* task = new RenderTask(this, m_backend, m_cache, pageNumber, targetSize, reqKey);
    m_threadPool.start(task, priority);
}

void PdfRenderQueue::preloadSurroundings(int currentPage, const QSize& targetSize, int pageCount) {
    if (pageCount <= 0 || targetSize.isEmpty()) {
        return;
    }

    // Preload order: current-1, current+1, current+2, current-2
    const int offsets[] = { -1, 1, 2, -2 };
    for (int offset : offsets) {
        int targetPage = currentPage + offset;
        if (targetPage >= 1 && targetPage <= pageCount) {
            requestPage(targetPage, targetSize, -1); // lower priority for preloads
        }
    }
}

void PdfRenderQueue::cancelAll() {
    m_threadPool.clear();
    m_threadPool.waitForDone(100);
    QMutexLocker locker(&m_mutex);
    m_activeRequests.clear();
}

} // namespace EtapKitap
