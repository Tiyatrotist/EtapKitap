#pragma once

#include "document/IDocument.h"
#include "annotations/AnnotationModel.h"
#include "storage/AnnotationRepository.h"
#include <QtCore/QString>

namespace EtapKitap {

class PdfExporter {
public:
    static bool exportAnnotatedPdf(IDocument* doc,
                                  AnnotationRepository* annRepo,
                                  const QString& outputFilePath,
                                  QString* outError = nullptr);
};

} // namespace EtapKitap
