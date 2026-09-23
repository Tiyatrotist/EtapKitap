#include "document/DocumentController.h"
#include "document/PdfDocument.h"
#include "pdf/FallbackPdfBackend.h"
#include "pdf/QtPdfBackend.h"
#include "core/Logger.h"

namespace EtapKitap {

DocumentController::DocumentController(QObject* parent)
    : QObject(parent)
    , m_pageCache(std::make_unique<PdfPageCache>())
    , m_pageModel(std::make_unique<PageModel>())
{
#ifdef ETAPKITAP_HAS_QTPDF
    m_pdfBackend = std::make_unique<QtPdfBackend>();
#else
    m_pdfBackend = std::make_unique<FallbackPdfBackend>();
#endif

    m_renderQueue = std::make_unique<PdfRenderQueue>(m_pdfBackend.get(), m_pageCache.get(), this);

    connect(m_renderQueue.get(), &PdfRenderQueue::pageRendered, this, [this](int page, const QSize& sz, const QImage&) {
        emit pageRenderReady(page, sz);
    });
}

DocumentController::~DocumentController() {
    closeDocument();
}

bool DocumentController::openFile(const QString& filePath) {
    closeDocument();

    qCInfo(logCore) << "Opening file:" << filePath;

    bool loaded = false;
#ifdef ETAPKITAP_HAS_QTPDF
    if (m_pdfBackend) {
        loaded = m_pdfBackend->load(filePath);
    }
#endif

    if (!loaded) {
        // Fallback software backend
        m_pdfBackend = std::make_unique<FallbackPdfBackend>();
        m_renderQueue = std::make_unique<PdfRenderQueue>(m_pdfBackend.get(), m_pageCache.get(), this);
        connect(m_renderQueue.get(), &PdfRenderQueue::pageRendered, this, [this](int page, const QSize& sz, const QImage&) {
            emit pageRenderReady(page, sz);
        });
        loaded = m_pdfBackend->load(filePath);
    }

    if (!loaded) {
        qCCritical(logCore) << "Failed to open document:" << filePath;
        return false;
    }

    m_document = std::make_unique<PdfDocument>(m_pdfBackend.get(), filePath);
    m_currentPage = 1;
    updateSecondaryPage();

    m_pageModel->setDocument(m_document.get());

    emit hasDocumentChanged(true);
    emit pageCountChanged(m_document->pageCount());
    emit titleChanged(m_document->title());
    emit currentPageChanged(m_currentPage);
    emit documentLoaded(m_document->id(), m_document->title());

    return true;
}

void DocumentController::closeDocument() {
    if (m_renderQueue) {
        m_renderQueue->cancelAll();
    }
    if (m_pageCache) {
        m_pageCache->clear();
    }
    if (m_pageModel) {
        m_pageModel->clear();
    }
    if (m_pdfBackend) {
        m_pdfBackend->close();
    }
    m_document.reset();
    m_currentPage = 1;
    m_secondaryPage = 0;

    emit hasDocumentChanged(false);
    emit pageCountChanged(0);
    emit titleChanged(QString());
    emit currentPageChanged(1);
    emit secondaryPageChanged(0);
}

int DocumentController::currentPage() const { return m_currentPage; }

void DocumentController::setCurrentPage(int page) {
    const int total = pageCount();
    if (total <= 0) return;

    int clamped = std::clamp(page, 1, total);
    if (m_viewMode == ViewMode::DualPage && clamped > 1 && (clamped % 2 != 0)) {
        // In dual page mode (assuming page 1 is cover), pages are paired (2,3), (4,5)
        clamped = clamped - 1;
    }

    if (m_currentPage != clamped) {
        m_currentPage = clamped;
        updateSecondaryPage();
        emit currentPageChanged(m_currentPage);
    }
}

int DocumentController::secondaryPage() const { return m_secondaryPage; }

void DocumentController::updateSecondaryPage() {
    int prevSecondary = m_secondaryPage;
    if (m_viewMode == ViewMode::DualPage && m_currentPage > 1) {
        int candidate = m_currentPage + 1;
        m_secondaryPage = (candidate <= pageCount()) ? candidate : 0;
    } else {
        m_secondaryPage = 0;
    }

    if (prevSecondary != m_secondaryPage) {
        emit secondaryPageChanged(m_secondaryPage);
    }
}

int DocumentController::pageCount() const {
    return m_document ? m_document->pageCount() : 0;
}

qreal DocumentController::zoom() const { return m_zoom; }

void DocumentController::setZoom(qreal zoom) {
    qreal clamped = std::clamp(zoom, 0.5, 4.0);
    if (!qFuzzyCompare(m_zoom, clamped)) {
        m_zoom = clamped;
        emit zoomChanged(m_zoom);
    }
}

ViewMode DocumentController::viewMode() const { return m_viewMode; }

void DocumentController::setViewMode(ViewMode mode) {
    if (m_viewMode != mode) {
        m_viewMode = mode;
        updateSecondaryPage();
        emit viewModeChanged(m_viewMode);
    }
}

int DocumentController::viewModeAsInt() const {
    return static_cast<int>(m_viewMode);
}

void DocumentController::setViewModeAsInt(int mode) {
    setViewMode(static_cast<ViewMode>(mode));
}

bool DocumentController::hasDocument() const {
    return m_document != nullptr && m_document->pageCount() > 0;
}

QString DocumentController::title() const {
    return m_document ? m_document->title() : QString();
}

IDocument* DocumentController::currentDocument() const {
    return m_document.get();
}

PageModel* DocumentController::pageModel() {
    return m_pageModel.get();
}

PdfPageCache* DocumentController::pageCache() {
    return m_pageCache.get();
}

PdfRenderQueue* DocumentController::renderQueue() {
    return m_renderQueue.get();
}

void DocumentController::nextPage() {
    int step = (m_viewMode == ViewMode::DualPage && m_currentPage > 1) ? 2 : 1;
    setCurrentPage(m_currentPage + step);
}

void DocumentController::previousPage() {
    int step = (m_viewMode == ViewMode::DualPage && m_currentPage > 2) ? 2 : 1;
    setCurrentPage(m_currentPage - step);
}

void DocumentController::firstPage() {
    setCurrentPage(1);
}

void DocumentController::lastPage() {
    setCurrentPage(pageCount());
}

void DocumentController::zoomIn() {
    setZoom(m_zoom * 1.25);
}

void DocumentController::zoomOut() {
    setZoom(m_zoom / 1.25);
}

void DocumentController::resetZoom() {
    setZoom(1.0);
}

void DocumentController::requestPageRender(int pageNumber, const QSize& targetPixelSize) {
    if (m_renderQueue) {
        m_renderQueue->requestPage(pageNumber, targetPixelSize);
        m_renderQueue->preloadSurroundings(pageNumber, targetPixelSize, pageCount());
    }
}

QImage DocumentController::getCachedPageImage(int pageNumber, const QSize& targetPixelSize) {
    if (m_pageCache) {
        return m_pageCache->get(pageNumber, targetPixelSize);
    }
    return QImage();
}

QImage DocumentController::getAnyResolutionImage(int pageNumber) const {
    if (m_pageCache) {
        return m_pageCache->getAnyResolution(pageNumber);
    }
    return QImage();
}

} // namespace EtapKitap
