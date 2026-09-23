#pragma once

#include "core/Types.h"
#include <QtCore/QObject>

namespace EtapKitap {

class ReaderSessionState : public QObject {
    Q_OBJECT
    Q_PROPERTY(int activeTool READ activeToolAsInt WRITE setActiveToolAsInt NOTIFY activeToolChanged)
    Q_PROPERTY(bool presentationMode READ presentationMode WRITE setPresentationMode NOTIFY presentationModeChanged)
    Q_PROPERTY(bool leftPanelOpen READ leftPanelOpen WRITE setLeftPanelOpen NOTIFY leftPanelOpenChanged)
    Q_PROPERTY(int activeNavTab READ activeNavTab WRITE setActiveNavTab NOTIFY activeNavTabChanged)
    Q_PROPERTY(int toolDockSide READ toolDockSide WRITE setToolDockSide NOTIFY toolDockSideChanged)
    Q_PROPERTY(bool annotationsVisible READ annotationsVisible WRITE setAnnotationsVisible NOTIFY annotationsVisibleChanged)
    Q_PROPERTY(bool interactionsVisible READ interactionsVisible WRITE setInteractionsVisible NOTIFY interactionsVisibleChanged)
    Q_PROPERTY(bool scratchpadOpen READ scratchpadOpen WRITE setScratchpadOpen NOTIFY scratchpadOpenChanged)
    Q_PROPERTY(bool whiteboardOpen READ whiteboardOpen WRITE setWhiteboardOpen NOTIFY whiteboardOpenChanged)
    Q_PROPERTY(bool freezeOpen READ freezeOpen WRITE setFreezeOpen NOTIFY freezeOpenChanged)
    Q_PROPERTY(bool teacherMode READ teacherMode WRITE setTeacherMode NOTIFY teacherModeChanged)

public:
    explicit ReaderSessionState(QObject* parent = nullptr);
    ~ReaderSessionState() override = default;

    [[nodiscard]] ToolType activeTool() const;
    void setActiveTool(ToolType tool);
    [[nodiscard]] int activeToolAsInt() const;
    void setActiveToolAsInt(int t);

    [[nodiscard]] bool presentationMode() const;
    void setPresentationMode(bool pm);
    Q_INVOKABLE void togglePresentationMode();

    [[nodiscard]] bool leftPanelOpen() const;
    void setLeftPanelOpen(bool open);
    Q_INVOKABLE void toggleLeftPanel();

    [[nodiscard]] int activeNavTab() const;
    void setActiveNavTab(int tab);

    [[nodiscard]] int toolDockSide() const; // 0: Left, 1: Right
    void setToolDockSide(int side);

    [[nodiscard]] bool annotationsVisible() const;
    void setAnnotationsVisible(bool v);
    Q_INVOKABLE void toggleAnnotationsVisible();

    [[nodiscard]] bool interactionsVisible() const;
    void setInteractionsVisible(bool v);
    Q_INVOKABLE void toggleInteractionsVisible();

    [[nodiscard]] bool scratchpadOpen() const;
    void setScratchpadOpen(bool open);
    Q_INVOKABLE void toggleScratchpad();

    [[nodiscard]] bool whiteboardOpen() const;
    void setWhiteboardOpen(bool open);
    Q_INVOKABLE void toggleWhiteboard();

    [[nodiscard]] bool freezeOpen() const;
    void setFreezeOpen(bool open);
    Q_INVOKABLE void toggleFreeze();

    [[nodiscard]] bool teacherMode() const;
    void setTeacherMode(bool tm);
    Q_INVOKABLE void toggleTeacherMode();

signals:
    void activeToolChanged(ToolType tool);
    void presentationModeChanged(bool pm);
    void leftPanelOpenChanged(bool open);
    void activeNavTabChanged(int tab);
    void toolDockSideChanged(int side);
    void annotationsVisibleChanged(bool v);
    void interactionsVisibleChanged(bool v);
    void scratchpadOpenChanged(bool open);
    void whiteboardOpenChanged(bool open);
    void freezeOpenChanged(bool open);
    void teacherModeChanged(bool tm);

private:
    ToolType m_activeTool{ToolType::Pointer};
    bool m_presentationMode{false};
    bool m_leftPanelOpen{false};
    int m_activeNavTab{0};
    int m_toolDockSide{1}; // 1 = Right dock
    bool m_annotationsVisible{true};
    bool m_interactionsVisible{true};
    bool m_scratchpadOpen{false};
    bool m_whiteboardOpen{false};
    bool m_freezeOpen{false};
    bool m_teacherMode{false};
};

} // namespace EtapKitap
