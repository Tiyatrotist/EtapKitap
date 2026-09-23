#include "storage/AnnotationRepository.h"
#include "annotations/StrokeAnnotation.h"
#include "annotations/ShapeAnnotation.h"
#include "core/Logger.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QJsonDocument>
#include <QtCore/QDateTime>

namespace EtapKitap {

AnnotationRepository::AnnotationRepository(DatabaseManager* dbManager)
    : m_dbManager(dbManager)
{
}

bool AnnotationRepository::savePageAnnotations(const QString& documentId, int pageNumber, const QVector<std::shared_ptr<Annotation>>& annotations) {
    if (!m_dbManager || !m_dbManager->isOpen() || documentId.isEmpty()) {
        return false;
    }

    auto db = m_dbManager->database();
    db.transaction();

    // Remove old annotations for this page
    QSqlQuery delQ(db);
    delQ.prepare(QStringLiteral("DELETE FROM annotations WHERE document_id = :docId AND page_number = :page;"));
    delQ.bindValue(QStringLiteral(":docId"), documentId);
    delQ.bindValue(QStringLiteral(":page"), pageNumber);
    if (!delQ.exec()) {
        db.rollback();
        return false;
    }

    // Insert active annotations
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    QSqlQuery insQ(db);
    insQ.prepare(QStringLiteral(
        "INSERT INTO annotations (id, document_id, page_number, tool_type, data_json, created_at, updated_at) "
        "VALUES (:id, :docId, :page, :type, :data, :created, :updated);"
    ));

    for (const auto& ann : annotations) {
        if (!ann) continue;
        insQ.bindValue(QStringLiteral(":id"), ann->id());
        insQ.bindValue(QStringLiteral(":docId"), documentId);
        insQ.bindValue(QStringLiteral(":page"), pageNumber);
        insQ.bindValue(QStringLiteral(":type"), static_cast<int>(ann->type()));
        insQ.bindValue(QStringLiteral(":data"), QString::fromUtf8(QJsonDocument(ann->toJson()).toJson(QJsonDocument::Compact)));
        insQ.bindValue(QStringLiteral(":created"), now);
        insQ.bindValue(QStringLiteral(":updated"), now);

        if (!insQ.exec()) {
            qCWarning(logStorage) << "Failed to insert annotation:" << insQ.lastError().text();
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

QVector<std::shared_ptr<Annotation>> AnnotationRepository::loadPageAnnotations(const QString& documentId, int pageNumber) {
    QVector<std::shared_ptr<Annotation>> list;
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return list;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("SELECT data_json, tool_type FROM annotations WHERE document_id = :docId AND page_number = :page ORDER BY rowid ASC;"));
    q.bindValue(QStringLiteral(":docId"), documentId);
    q.bindValue(QStringLiteral(":page"), pageNumber);

    if (q.exec()) {
        while (q.next()) {
            QByteArray jsonBytes = q.value(0).toByteArray();
            QJsonObject obj = QJsonDocument::fromJson(jsonBytes).object();
            ToolType t = static_cast<ToolType>(q.value(1).toInt());

            if (t == ToolType::Pen || t == ToolType::Highlighter) {
                auto stroke = StrokeAnnotation::fromJson(obj);
                if (stroke) list.append(std::move(stroke));
            } else if (t == ToolType::Line || t == ToolType::Arrow || t == ToolType::Rectangle || t == ToolType::Ellipse) {
                auto shape = ShapeAnnotation::fromJson(obj);
                if (shape) list.append(std::move(shape));
            }
        }
    }
    return list;
}

QVector<std::shared_ptr<Annotation>> AnnotationRepository::loadAllDocumentAnnotations(const QString& documentId) {
    QVector<std::shared_ptr<Annotation>> list;
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return list;
    }

    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("SELECT data_json, tool_type FROM annotations WHERE document_id = :docId ORDER BY rowid ASC;"));
    q.bindValue(QStringLiteral(":docId"), documentId);

    if (q.exec()) {
        while (q.next()) {
            QByteArray jsonBytes = q.value(0).toByteArray();
            QJsonObject obj = QJsonDocument::fromJson(jsonBytes).object();
            ToolType t = static_cast<ToolType>(q.value(1).toInt());

            if (t == ToolType::Pen || t == ToolType::Highlighter) {
                auto stroke = StrokeAnnotation::fromJson(obj);
                if (stroke) list.append(std::move(stroke));
            } else if (t == ToolType::Line || t == ToolType::Arrow || t == ToolType::Rectangle || t == ToolType::Ellipse) {
                auto shape = ShapeAnnotation::fromJson(obj);
                if (shape) list.append(std::move(shape));
            }
        }
    }
    return list;
}

bool AnnotationRepository::deleteDocumentAnnotations(const QString& documentId) {
    if (!m_dbManager || !m_dbManager->isOpen()) {
        return false;
    }
    QSqlQuery q(m_dbManager->database());
    q.prepare(QStringLiteral("DELETE FROM annotations WHERE document_id = :docId;"));
    q.bindValue(QStringLiteral(":docId"), documentId);
    return q.exec();
}

} // namespace EtapKitap
