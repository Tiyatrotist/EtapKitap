#pragma once

#include "document/IDocument.h"
#include "document/PageModel.h"
#include "document/DocumentSession.h"
#include "pdf/IPdfBackend.h"
#include "pdf/PdfPageCache.h"
#include "pdf/PdfRenderQueue.h"
#include <QtCore/QObject>
#include <memory>

namespace EtapKitap {

class DocumentController : public QObject {
    Q_OBJECT
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int secondaryPage READ secondaryPage NOTIFY secondaryPageChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(int viewMode READ viewModeAsInt WRITE setViewModeAsInt NOTIFY viewModeChanged)
    Q_PROPERTY(bool hasDocument READ hasDocument NOTIFY hasDocumentChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(PageModel* pageModel READ pageModel CONSTANT)

public:
    explicit DocumentController(QObject* parent = nullptr);
    ~DocumentController() override;

    bool openFile(const QString& filePath);
    void closeDocument();

    [[nodiscard]] int currentPage() const;
    void setCurrentPage(int page);

    [[nodiscard]] int secondaryPage() const;
    [[nodiscard]] int pageCount() const;

    [[nodiscard]] qreal zoom() const;
    void setZoom(qreal zoom);

    [[nodiscard]] ViewMode viewMode() const;
    void setViewMode(ViewMode mode);
    [[nodiscard]] int viewModeAsInt() const;
    void setViewModeAsInt(int mode);

    [[nodiscard]] bool hasDocument() const;
    [[nodiscard]] QString title() const;
    [[nodiscard]] IDocument* currentDocument() const;
    [[nodiscard]] PageModel* pageModel();
    [[nodiscard]] PdfPageCache* pageCache();
    [[nodiscard]] PdfRenderQueue* renderQueue();

    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void previousPage();
    Q_INVOKABLE void firstPage();
    Q_INVOKABLE void lastPage();
    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE void resetZoom();

    void requestPageRender(int pageNumber, const QSize& targetPixelSize);
    [[nodiscard]] QImage getCachedPageImage(int pageNumber, const QSize& targetPixelSize);
    [[nodiscard]] QImage getAnyResolutionImage(int pageNumber) const;

signals:
    void currentPageChanged(int page);
    void secondaryPageChanged(int page);
    void pageCountChanged(int count);
    void zoomChanged(qreal zoom);
    void viewModeChanged(ViewMode mode);
    void hasDocumentChanged(bool hasDoc);
    void titleChanged(const QString& title);
    void documentLoaded(const QString& docId, const QString& title);
    void pageRenderReady(int pageNumber, const QSize& size);

private:
    void updateSecondaryPage();

    std::unique_ptr<IPdfBackend> m_pdfBackend;
    std::unique_ptr<PdfPageCache> m_pageCache;
    std::unique_ptr<PdfRenderQueue> m_renderQueue;
    std::unique_ptr<PageModel> m_pageModel;
    std::unique_ptr<IDocument> m_document;

    int m_currentPage{1};
    int m_secondaryPage{0};
    qreal m_zoom{1.0};
    ViewMode m_viewMode{ViewMode::SinglePage};
};

} // namespace EtapKitap
