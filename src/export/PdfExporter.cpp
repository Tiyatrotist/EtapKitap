#include "export/PdfExporter.h"
#include "core/Logger.h"
#include <QtGui/QPdfWriter>
#include <QtGui/QPainter>
#include <QtGui/QPageSize>

namespace EtapKitap {

bool PdfExporter::exportAnnotatedPdf(IDocument* doc,
                                     AnnotationRepository* annRepo,
                                     const QString& outputFilePath,
                                     QString* outError)
{
    if (!doc || doc->pageCount() <= 0) {
        if (outError) *outError = QStringLiteral("Doküman bulunamadı veya boş.");
        return false;
    }

    QPdfWriter writer(outputFilePath);
    writer.setCreator(QStringLiteral("EtapKitap"));
    writer.setTitle(doc->title() + QStringLiteral(" (Açıklamalı)"));

    QPainter painter;
    if (!painter.begin(&writer)) {
        if (outError) *outError = QStringLiteral("PDF yazıcı başlatılamadı: ") + outputFilePath;
        return false;
    }

    const int total = doc->pageCount();
    for (int p = 1; p <= total; ++p) {
        if (p > 1) {
            writer.newPage();
        }

        const QSizeF ptSize = doc->pageSize(p);
        writer.setPageSize(QPageSize(ptSize, QPageSize::Point));

        const QRect targetRect = painter.viewport();
        const qreal w = targetRect.width();
        const qreal h = targetRect.height();

        // Load annotations for this page
        if (annRepo) {
            auto anns = annRepo->loadPageAnnotations(doc->id(), p);
            for (const auto& ann : anns) {
                if (ann) {
                    ann->render(painter, w, h);
                }
            }
        }
    }

    painter.end();
    qCInfo(logCore) << "Exported annotated PDF to:" << outputFilePath;
    return true;
}

} // namespace EtapKitap
