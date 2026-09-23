#pragma once

#include "annotations/Annotation.h"
#include "annotations/AnnotationModel.h"
#include <QtGui/QUndoCommand>
#include <memory>

namespace EtapKitap {

class AddAnnotationCommand : public QUndoCommand {
public:
    AddAnnotationCommand(AnnotationModel* model, std::shared_ptr<Annotation> annotation, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;

private:
    AnnotationModel* m_model;
    std::shared_ptr<Annotation> m_annotation;
};

class RemoveAnnotationCommand : public QUndoCommand {
public:
    RemoveAnnotationCommand(AnnotationModel* model, std::shared_ptr<Annotation> annotation, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;

private:
    AnnotationModel* m_model;
    std::shared_ptr<Annotation> m_annotation;
};

class ClearPageAnnotationsCommand : public QUndoCommand {
public:
    ClearPageAnnotationsCommand(AnnotationModel* model, int pageNumber, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;

private:
    AnnotationModel* m_model;
    int m_pageNumber;
    QVector<std::shared_ptr<Annotation>> m_removed;
};

} // namespace EtapKitap
