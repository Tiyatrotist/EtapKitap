#pragma once

#include "pdf/IPdfBackend.h"
#include "pdf/PdfPageCache.h"
#include <QtCore/QObject>
#include <QtCore/QThreadPool>
#include <QtCore/QSet>
#include <QtCore/QMutex>

namespace EtapKitap {

class PdfRenderQueue : public QObject {
    Q_OBJECT

public:
    explicit PdfRenderQueue(IPdfBackend* backend, PdfPageCache* cache, QObject* parent = nullptr);
    ~PdfRenderQueue() override;

    void requestPage(int pageNumber, const QSize& targetSize, int priority = 0);
    void preloadSurroundings(int currentPage, const QSize& targetSize, int pageCount);
    void cancelAll();

signals:
    void pageRendered(int pageNumber, const QSize& targetSize, const QImage& image);

private:
    IPdfBackend* m_backend{nullptr};
    PdfPageCache* m_cache{nullptr};
    QThreadPool m_threadPool;
    QMutex m_mutex;
    QSet<QString> m_activeRequests;
};

} // namespace EtapKitap
