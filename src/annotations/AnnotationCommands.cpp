#include "annotations/AnnotationCommands.h"

namespace EtapKitap {

// AddAnnotationCommand
AddAnnotationCommand::AddAnnotationCommand(AnnotationModel* model, std::shared_ptr<Annotation> annotation, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_annotation(annotation)
{
    setText(QStringLiteral("Cizim Ekle"));
}

void AddAnnotationCommand::undo() {
    if (m_model && m_annotation) {
        m_model->removeAnnotation(m_annotation->id());
    }
}

void AddAnnotationCommand::redo() {
    if (m_model && m_annotation) {
        m_model->addAnnotation(m_annotation);
    }
}

// RemoveAnnotationCommand
RemoveAnnotationCommand::RemoveAnnotationCommand(AnnotationModel* model, std::shared_ptr<Annotation> annotation, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_annotation(annotation)
{
    setText(QStringLiteral("Cizim Sil"));
}

void RemoveAnnotationCommand::undo() {
    if (m_model && m_annotation) {
        m_model->addAnnotation(m_annotation);
    }
}

void RemoveAnnotationCommand::redo() {
    if (m_model && m_annotation) {
        m_model->removeAnnotation(m_annotation->id());
    }
}

// ClearPageAnnotationsCommand
ClearPageAnnotationsCommand::ClearPageAnnotationsCommand(AnnotationModel* model, int pageNumber, QUndoCommand* parent)
    : QUndoCommand(parent)
    , m_model(model)
    , m_pageNumber(pageNumber)
{
    setText(QStringLiteral("Sayfayi Temizle"));
    if (m_model) {
        m_removed = m_model->annotationsForPage(m_pageNumber);
    }
}

void ClearPageAnnotationsCommand::undo() {
    if (m_model) {
        for (const auto& ann : m_removed) {
            m_model->addAnnotation(ann);
        }
    }
}

void ClearPageAnnotationsCommand::redo() {
    if (m_model) {
        m_model->clearPage(m_pageNumber);
    }
}

} // namespace EtapKitap
