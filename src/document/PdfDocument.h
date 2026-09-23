#pragma once

#include "document/IDocument.h"
#include "pdf/IPdfBackend.h"
#include <QtCore/QCryptographicHash>
#include <QtCore/QFileInfo>

namespace EtapKitap {

class PdfDocument : public IDocument {
public:
    explicit PdfDocument(IPdfBackend* backend, const QString& filePath);
    ~PdfDocument() override = default;

    [[nodiscard]] QString id() const override;
    [[nodiscard]] QString title() const override;
    [[nodiscard]] QString filePath() const override;
    [[nodiscard]] int pageCount() const override;
    [[nodiscard]] QSizeF pageSize(int pageNumber) const override;
    [[nodiscard]] bool isInteractiveBook() const override;
    [[nodiscard]] QString pageText(int pageNumber) const override;

private:
    IPdfBackend* m_backend{nullptr};
    QString m_filePath;
    QString m_id;
    QString m_title;
};

} // namespace EtapKitap
