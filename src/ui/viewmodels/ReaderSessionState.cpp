#include "ui/viewmodels/ReaderSessionState.h"

namespace EtapKitap {

ReaderSessionState::ReaderSessionState(QObject* parent)
    : QObject(parent)
{
}

ToolType ReaderSessionState::activeTool() const { return m_activeTool; }
void ReaderSessionState::setActiveTool(ToolType tool) {
    if (m_activeTool != tool) {
        m_activeTool = tool;
        emit activeToolChanged(m_activeTool);
    }
}

int ReaderSessionState::activeToolAsInt() const {
    return static_cast<int>(m_activeTool);
}
void ReaderSessionState::setActiveToolAsInt(int t) {
    setActiveTool(static_cast<ToolType>(t));
}

bool ReaderSessionState::presentationMode() const { return m_presentationMode; }
void ReaderSessionState::setPresentationMode(bool pm) {
    if (m_presentationMode != pm) {
        m_presentationMode = pm;
        if (m_presentationMode) {
            // Presentation mode closes side navigation
            setLeftPanelOpen(false);
        }
        emit presentationModeChanged(m_presentationMode);
    }
}
void ReaderSessionState::togglePresentationMode() {
    setPresentationMode(!m_presentationMode);
}

bool ReaderSessionState::leftPanelOpen() const { return m_leftPanelOpen; }
void ReaderSessionState::setLeftPanelOpen(bool open) {
    if (m_leftPanelOpen != open) {
        m_leftPanelOpen = open;
        emit leftPanelOpenChanged(m_leftPanelOpen);
    }
}
void ReaderSessionState::toggleLeftPanel() {
    setLeftPanelOpen(!m_leftPanelOpen);
}

int ReaderSessionState::activeNavTab() const { return m_activeNavTab; }
void ReaderSessionState::setActiveNavTab(int tab) {
    if (m_activeNavTab != tab) {
        m_activeNavTab = tab;
        emit activeNavTabChanged(m_activeNavTab);
    }
}

int ReaderSessionState::toolDockSide() const { return m_toolDockSide; }
void ReaderSessionState::setToolDockSide(int side) {
    if (m_toolDockSide != side) {
        m_toolDockSide = side;
        emit toolDockSideChanged(m_toolDockSide);
    }
}

bool ReaderSessionState::annotationsVisible() const { return m_annotationsVisible; }
void ReaderSessionState::setAnnotationsVisible(bool v) {
    if (m_annotationsVisible != v) {
        m_annotationsVisible = v;
        emit annotationsVisibleChanged(m_annotationsVisible);
    }
}
void ReaderSessionState::toggleAnnotationsVisible() {
    setAnnotationsVisible(!m_annotationsVisible);
}

bool ReaderSessionState::interactionsVisible() const { return m_interactionsVisible; }
void ReaderSessionState::setInteractionsVisible(bool v) {
    if (m_interactionsVisible != v) {
        m_interactionsVisible = v;
        emit interactionsVisibleChanged(m_interactionsVisible);
    }
}
void ReaderSessionState::toggleInteractionsVisible() {
    setInteractionsVisible(!m_interactionsVisible);
}

bool ReaderSessionState::scratchpadOpen() const { return m_scratchpadOpen; }
void ReaderSessionState::setScratchpadOpen(bool open) {
    if (m_scratchpadOpen != open) {
        m_scratchpadOpen = open;
        emit scratchpadOpenChanged(m_scratchpadOpen);
    }
}
void ReaderSessionState::toggleScratchpad() {
    setScratchpadOpen(!m_scratchpadOpen);
}

bool ReaderSessionState::whiteboardOpen() const { return m_whiteboardOpen; }
void ReaderSessionState::setWhiteboardOpen(bool open) {
    if (m_whiteboardOpen != open) {
        m_whiteboardOpen = open;
        emit whiteboardOpenChanged(m_whiteboardOpen);
    }
}
void ReaderSessionState::toggleWhiteboard() {
    setWhiteboardOpen(!m_whiteboardOpen);
}

bool ReaderSessionState::freezeOpen() const { return m_freezeOpen; }
void ReaderSessionState::setFreezeOpen(bool open) {
    if (m_freezeOpen != open) {
        m_freezeOpen = open;
        emit freezeOpenChanged(m_freezeOpen);
    }
}
void ReaderSessionState::toggleFreeze() {
    setFreezeOpen(!m_freezeOpen);
}

bool ReaderSessionState::teacherMode() const { return m_teacherMode; }
void ReaderSessionState::setTeacherMode(bool tm) {
    if (m_teacherMode != tm) {
        m_teacherMode = tm;
        emit teacherModeChanged(m_teacherMode);
    }
}
void ReaderSessionState::toggleTeacherMode() {
    setTeacherMode(!m_teacherMode);
}

} // namespace EtapKitap
