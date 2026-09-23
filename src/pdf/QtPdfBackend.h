#pragma once

#include "pdf/IPdfBackend.h"

#ifdef ETAPKITAP_HAS_QTPDF
#include <QtPdf/QPdfDocument>
#endif

namespace EtapKitap {

class QtPdfBackend : public IPdfBackend {
public:
    QtPdfBackend();
    ~QtPdfBackend() override;

    bool load(const QString& filePath) override;
    void close() override;
    [[nodiscard]] bool isLoaded() const override;

    [[nodiscard]] int pageCount() const override;
    [[nodiscard]] QSizeF pageSize(int pageNumber) const override;
    [[nodiscard]] QImage renderPage(int pageNumber, const QSize& targetPixelSize) override;
    [[nodiscard]] QString pageText(int pageNumber) const override;
    [[nodiscard]] QString title() const override;

private:
#ifdef ETAPKITAP_HAS_QTPDF
    std::unique_ptr<QPdfDocument> m_doc;
#endif
    bool m_isLoaded{false};
    QString m_filePath;
};

} // namespace EtapKitap
