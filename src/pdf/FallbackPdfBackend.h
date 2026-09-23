#pragma once

#include "pdf/IPdfBackend.h"
#include <QtCore/QFileInfo>
#include <QtCore/QVector>

namespace EtapKitap {

/**
 * @brief High quality software/procedural vector PDF backend used when QtPdf is not present.
 * Renders textbook pages with crisp typography, diagrams, chapter headings, and margins.
 */
class FallbackPdfBackend : public IPdfBackend {
public:
    explicit FallbackPdfBackend();
    ~FallbackPdfBackend() override = default;

    bool load(const QString& filePath) override;
    void close() override;
    [[nodiscard]] bool isLoaded() const override;

    [[nodiscard]] int pageCount() const override;
    [[nodiscard]] QSizeF pageSize(int pageNumber) const override;
    [[nodiscard]] QImage renderPage(int pageNumber, const QSize& targetPixelSize) override;
    [[nodiscard]] QString pageText(int pageNumber) const override;
    [[nodiscard]] QString title() const override;

    void setPageCount(int count);
    void setTitle(const QString& title);

private:
    bool m_isLoaded{false};
    QString m_filePath;
    QString m_title{"Etkilesimli Ders Kitabi"};
    int m_pageCount{12};
    QSizeF m_standardPageSize{595.0, 842.0}; // Standard A4 points
};

} // namespace EtapKitap
