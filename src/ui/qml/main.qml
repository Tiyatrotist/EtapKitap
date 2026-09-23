import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "views"
import "components"

ApplicationWindow {
    id: appWindow

    width: 1366
    height: 768
    visible: true
    title: "EtapKitap — " + (appCtrl.docCtrl.hasDocument ? appCtrl.docCtrl.title : "Pardus ETAP Dijital Ders Kitabı")

    // Background color
    color: "#F1F5F9"

    // Stack between HomeView (Library) and ReaderView
    Item {
        anchors.fill: parent

        HomeView {
            anchors.fill: parent
            visible: !appCtrl.docCtrl.hasDocument
        }

        ReaderView {
            anchors.fill: parent
            visible: appCtrl.docCtrl.hasDocument
        }

        // Floating Toast Notification
        ToastNotification {
            id: toast
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 70
        }
    }

    Connections {
        target: appCtrl
        function onToastMessage(msg) {
            toast.show(msg);
        }
    }

    // Keyboard Shortcuts
    Shortcut {
        sequence: "Ctrl+Z"
        onActivated: appCtrl.toolCtrl.undoManager.undo()
    }

    Shortcut {
        sequence: "Ctrl+Y"
        onActivated: appCtrl.toolCtrl.undoManager.redo()
    }

    Shortcut {
        sequence: "Ctrl+Shift+Z"
        onActivated: appCtrl.toolCtrl.undoManager.redo()
    }

    Shortcut {
        sequence: "F11"
        onActivated: appCtrl.sessionState.togglePresentationMode()
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            if (appCtrl.mediaModalOpen) {
                appCtrl.setMediaModalOpen(false);
            } else if (appCtrl.activityModel.isOpen) {
                appCtrl.activityModel.close();
            } else if (appCtrl.sessionState.whiteboardOpen) {
                appCtrl.sessionState.setWhiteboardOpen(false);
            } else if (appCtrl.sessionState.freezeOpen) {
                appCtrl.sessionState.setFreezeOpen(false);
            } else if (appCtrl.sessionState.presentationMode) {
                appCtrl.sessionState.setPresentationMode(false);
            }
        }
    }

    Shortcut {
        sequence: "Left"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.previousPage()
    }

    Shortcut {
        sequence: "Right"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.nextPage()
    }

    Shortcut {
        sequence: "PgUp"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.previousPage()
    }

    Shortcut {
        sequence: "PgDown"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.nextPage()
    }

    Shortcut {
        sequence: "Ctrl++"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.zoomIn()
    }

    Shortcut {
        sequence: "Ctrl+-"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.zoomOut()
    }

    Shortcut {
        sequence: "Ctrl+0"
        onActivated: if (appCtrl.docCtrl.hasDocument) appCtrl.docCtrl.resetZoom()
    }
}
