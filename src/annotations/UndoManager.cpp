#include "annotations/UndoManager.h"

namespace EtapKitap {

UndoManager::UndoManager(QObject* parent)
    : QObject(parent)
{
    connect(&m_stack, &QUndoStack::canUndoChanged, this, &UndoManager::canUndoChanged);
    connect(&m_stack, &QUndoStack::canRedoChanged, this, &UndoManager::canRedoChanged);
    connect(&m_stack, &QUndoStack::undoTextChanged, this, &UndoManager::undoTextChanged);
    connect(&m_stack, &QUndoStack::redoTextChanged, this, &UndoManager::redoTextChanged);
}

bool UndoManager::canUndo() const { return m_stack.canUndo(); }
bool UndoManager::canRedo() const { return m_stack.canRedo(); }
QString UndoManager::undoText() const { return m_stack.undoText(); }
QString UndoManager::redoText() const { return m_stack.redoText(); }

void UndoManager::undo() { m_stack.undo(); }
void UndoManager::redo() { m_stack.redo(); }
void UndoManager::clear() { m_stack.clear(); }

void UndoManager::push(QUndoCommand* cmd) {
    if (cmd) {
        m_stack.push(cmd);
    }
}

} // namespace EtapKitap
