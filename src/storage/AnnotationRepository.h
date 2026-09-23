#pragma once

#include "annotations/Annotation.h"
#include "storage/DatabaseManager.h"
#include <QtCore/QVector>
#include <memory>

namespace EtapKitap {

class AnnotationRepository {
public:
    explicit AnnotationRepository(DatabaseManager* dbManager);

    bool savePageAnnotations(const QString& documentId, int pageNumber, const QVector<std::shared_ptr<Annotation>>& annotations);
    [[nodiscard]] QVector<std::shared_ptr<Annotation>> loadPageAnnotations(const QString& documentId, int pageNumber);
    [[nodiscard]] QVector<std::shared_ptr<Annotation>> loadAllDocumentAnnotations(const QString& documentId);
    bool deleteDocumentAnnotations(const QString& documentId);

private:
    DatabaseManager* m_dbManager;
};

} // namespace EtapKitap
