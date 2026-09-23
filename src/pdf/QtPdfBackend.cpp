#include "pdf/QtPdfBackend.h"
#include "core/Logger.h"
#include <QtCore/QFileInfo>

#ifdef ETAPKITAP_HAS_QTPDF
#include <QtPdf/QPdfDocumentRenderOptions>
#endif

namespace EtapKitap {

QtPdfBackend::QtPdfBackend() {
#ifdef ETAPKITAP_HAS_QTPDF
    m_doc = std::make_unique<QPdfDocument>();
#endif
}

QtPdfBackend::~QtPdfBackend() {
    close();
}

bool QtPdfBackend::load(const QString& filePath) {
    m_filePath = filePath;
#ifdef ETAPKITAP_HAS_QTPDF
    if (!m_doc) {
        m_doc = std::make_unique<QPdfDocument>();
    }
    const auto err = m_doc->load(filePath);
    if (err != QPdfDocument::Error::None) {
        qCCritical(logPdf) << "Failed to load PDF with QtPdf:" << filePath << "Error:" << static_cast<int>(err);
        m_isLoaded = false;
        return false;
    }
    m_isLoaded = true;
    qCInfo(logPdf) << "Loaded PDF with QtPdf:" << filePath << "Pages:" << m_doc->pageCount();
    return true;
#else
    qCWarning(logPdf) << "QtPdf not compiled in this build. Cannot load:" << filePath;
    m_isLoaded = false;
    return false;
#endif
}

void QtPdfBackend::close() {
#ifdef ETAPKITAP_HAS_QTPDF
    if (m_doc) {
        m_doc->close();
    }
#endif
    m_isLoaded = false;
    m_filePath.clear();
}

bool QtPdfBackend::isLoaded() const {
    return m_isLoaded;
}

int QtPdfBackend::pageCount() const {
#ifdef ETAPKITAP_HAS_QTPDF
    return (m_doc && m_isLoaded) ? m_doc->pageCount() : 0;
#else
    return 0;
#endif
}

QSizeF QtPdfBackend::pageSize(int pageNumber) const {
#ifdef ETAPKITAP_HAS_QTPDF
    if (m_doc && m_isLoaded && pageNumber >= 1 && pageNumber <= m_doc->pageCount()) {
        return m_doc->pagePointSize(pageNumber - 1);
    }
#endif
    return QSizeF(595.0, 842.0);
}

QImage QtPdfBackend::renderPage(int pageNumber, const QSize& targetPixelSize) {
#ifdef ETAPKITAP_HAS_QTPDF
    if (m_doc && m_isLoaded && pageNumber >= 1 && pageNumber <= m_doc->pageCount()) {
        QPdfDocumentRenderOptions options;
        return m_doc->render(pageNumber - 1, targetPixelSize, options);
    }
#endif
    Q_UNUSED(pageNumber);
    Q_UNUSED(targetPixelSize);
    return QImage();
}

QString QtPdfBackend::pageText(int pageNumber) const {
    Q_UNUSED(pageNumber);
    // Qt PDF text extraction is done via QPdfSelection / search model
    return QString();
}

QString QtPdfBackend::title() const {
#ifdef ETAPKITAP_HAS_QTPDF
    if (m_doc && m_isLoaded) {
        const QString docTitle = m_doc->metaData(QPdfDocument::MetaDataField::Title).toString();
        if (!docTitle.isEmpty()) {
            return docTitle;
        }
    }
#endif
    return QFileInfo(m_filePath).baseName();
}

} // namespace EtapKitap
