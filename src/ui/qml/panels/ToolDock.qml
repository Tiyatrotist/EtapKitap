import QtQuick
import QtQuick.Layouts
import "../components"

Rectangle {
    id: root

    width: 68
    color: "#FFFFFF"
    radius: 12
    border.color: "#E1E6EB"
    border.width: 1

    signal penPropertyRequested(real x, real y)
    signal eraserPropertyRequested(real x, real y)
    signal shapePropertyRequested(real x, real y)

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.bottomMargin: 8
        spacing: 4

        // 1. Kitap Grubu
        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "👆"
            tooltipText: "Etkileşim ve Gezinme Modu"
            active: appCtrl.sessionState.activeToolAsInt === 0
            onClicked: appCtrl.sessionState.setActiveToolAsInt(0)
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.leftMargin: 8
            Layout.rightMargin: 8
            height: 1
            color: "#E2E8F0"
        }

        // 2. Çizim Grubu
        IconButton {
            id: penBtn
            Layout.alignment: Qt.AlignHCenter
            iconText: "✏️"
            tooltipText: "Kalem (Özellikler için uzun basın)"
            active: appCtrl.sessionState.activeToolAsInt === 1
            onClicked: {
                if (appCtrl.sessionState.activeToolAsInt === 1) {
                    var p = penBtn.mapToItem(null, 0, 0);
                    root.penPropertyRequested(p.x, p.y);
                } else {
                    appCtrl.sessionState.setActiveToolAsInt(1);
                }
            }
            onLongPressed: {
                var p = penBtn.mapToItem(null, 0, 0);
                root.penPropertyRequested(p.x, p.y);
            }
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "🖍️"
            tooltipText: "Fosforlu Kalem"
            active: appCtrl.sessionState.activeToolAsInt === 2
            onClicked: appCtrl.sessionState.setActiveToolAsInt(2)
        }

        IconButton {
            id: eraserBtn
            Layout.alignment: Qt.AlignHCenter
            iconText: "🧹"
            tooltipText: "Silgi (Stroke / Precision)"
            active: appCtrl.sessionState.activeToolAsInt === 3
            onClicked: {
                if (appCtrl.sessionState.activeToolAsInt === 3) {
                    var p = eraserBtn.mapToItem(null, 0, 0);
                    root.eraserPropertyRequested(p.x, p.y);
                } else {
                    appCtrl.sessionState.setActiveToolAsInt(3);
                }
            }
            onLongPressed: {
                var p = eraserBtn.mapToItem(null, 0, 0);
                root.eraserPropertyRequested(p.x, p.y);
            }
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "📐"
            tooltipText: "Geometrik Şekiller (Çizgi, Ok, Dikdörtgen, Elips)"
            active: appCtrl.sessionState.activeToolAsInt >= 4 && appCtrl.sessionState.activeToolAsInt <= 7
            onClicked: appCtrl.sessionState.setActiveToolAsInt(6) // Rectangle default
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "↩️"
            tooltipText: "Geri Al (Ctrl+Z)"
            active: appCtrl.toolCtrl.undoManager.canUndo
            onClicked: appCtrl.toolCtrl.undoManager.undo()
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "↪️"
            tooltipText: "Yinele (Ctrl+Y)"
            active: appCtrl.toolCtrl.undoManager.canRedo
            onClicked: appCtrl.toolCtrl.undoManager.redo()
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: appCtrl.sessionState.annotationsVisible ? "👁️" : "🙈"
            tooltipText: appCtrl.sessionState.annotationsVisible ? "Çizimleri Gizle" : "Çizimleri Göster"
            onClicked: appCtrl.sessionState.toggleAnnotationsVisible()
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.leftMargin: 8
            Layout.rightMargin: 8
            height: 1
            color: "#E2E8F0"
        }

        // 3. Ders & Akıllı Tahta Grubu
        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "🔴"
            tooltipText: "Lazer İşaretçi"
            active: appCtrl.sessionState.activeToolAsInt === 8
            onClicked: {
                if (appCtrl.sessionState.activeToolAsInt === 8) {
                    appCtrl.sessionState.setActiveToolAsInt(0);
                    appCtrl.toolCtrl.laserPointer.setActive(false);
                } else {
                    appCtrl.sessionState.setActiveToolAsInt(8);
                    appCtrl.toolCtrl.laserPointer.setActive(true);
                }
            }
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "🔦"
            tooltipText: "Spotlight (Odaklama)"
            active: appCtrl.toolCtrl.spotlightTool.active
            onClicked: appCtrl.toolCtrl.spotlightTool.setActive(!appCtrl.toolCtrl.spotlightTool.active)
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "🎭"
            tooltipText: "Perde (Curtain Tool)"
            active: appCtrl.toolCtrl.curtainTool.active
            onClicked: appCtrl.toolCtrl.curtainTool.setActive(!appCtrl.toolCtrl.curtainTool.active)
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "📋"
            tooltipText: "Hızlı Beyaz Tahta"
            active: appCtrl.sessionState.whiteboardOpen
            onClicked: appCtrl.sessionState.toggleWhiteboard()
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "📝"
            tooltipText: "Çalışma Alanı (Scratchpad)"
            active: appCtrl.sessionState.scratchpadOpen
            onClicked: appCtrl.sessionState.toggleScratchpad()
        }

        IconButton {
            Layout.alignment: Qt.AlignHCenter
            iconText: "❄️"
            tooltipText: "Ekranı Dondur (Freeze Mode)"
            active: appCtrl.sessionState.freezeOpen
            onClicked: appCtrl.sessionState.toggleFreeze()
        }
    }
}
