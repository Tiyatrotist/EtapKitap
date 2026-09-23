#include "document/PdfDocument.h"

namespace EtapKitap {

PdfDocument::PdfDocument(IPdfBackend* backend, const QString& filePath)
    : m_backend(backend)
    , m_filePath(filePath)
{
    // Generate deterministic id from canonical file path
    QByteArray hash = QCryptographicHash::hash(filePath.toUtf8(), QCryptographicHash::Sha256);
    m_id = QString::fromLatin1(hash.toHex().left(24));

    if (m_backend) {
        m_title = m_backend->title();
    }
    if (m_title.isEmpty()) {
        m_title = QFileInfo(filePath).baseName();
    }
}

QString PdfDocument::id() const { return m_id; }
QString PdfDocument::title() const { return m_title; }
QString PdfDocument::filePath() const { return m_filePath; }

int PdfDocument::pageCount() const {
    return m_backend ? m_backend->pageCount() : 0;
}

QSizeF PdfDocument::pageSize(int pageNumber) const {
    return m_backend ? m_backend->pageSize(pageNumber) : QSizeF(595.0, 842.0);
}

bool PdfDocument::isInteractiveBook() const {
    return false;
}

QString PdfDocument::pageText(int pageNumber) const {
    return m_backend ? m_backend->pageText(pageNumber) : QString();
}

} // namespace EtapKitap
