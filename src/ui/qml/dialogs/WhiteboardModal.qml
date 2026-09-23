import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import EtapKitap 1.0
import "../components"

Rectangle {
    id: root

    visible: appCtrl.sessionState.whiteboardOpen
    anchors.fill: parent
    color: "#FFFFFF"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Whiteboard Control Header
        Rectangle {
            Layout.fillWidth: true
            height: 56
            color: "#1E293B"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 12

                Text {
                    text: "📋 Hızlı Beyaz Tahta"
                    font.bold: true
                    font.pixelSize: 16
                    color: "#FFFFFF"
                }

                Rectangle { width: 1; height: 24; color: "#475569" }

                // Template Switcher
                RowLayout {
                    spacing: 6
                    Repeater {
                        model: [
                            { name: "Kareli", val: 3 },
                            { name: "Çizgili", val: 2 },
                            { name: "Noktalı", val: 4 },
                            { name: "Koordinat", val: 5 },
                            { name: "Beyaz", val: 0 },
                            { name: "Siyah", val: 1 }
                        ]
                        Button {
                            text: modelData.name
                            highlighted: appCtrl.whiteboard.currentTemplate === modelData.val
                            onClicked: appCtrl.whiteboard.setCurrentTemplate(modelData.val)
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Page switcher
                RowLayout {
                    spacing: 8
                    Button {
                        text: "◀"
                        onClicked: appCtrl.whiteboard.previousPage()
                    }
                    Text {
                        text: appCtrl.whiteboard.currentPage + " / " + appCtrl.whiteboard.pageCount
                        color: "#FFFFFF"
                        font.bold: true
                    }
                    Button {
                        text: "▶"
                        onClicked: appCtrl.whiteboard.nextPage()
                    }
                    Button {
                        text: "+ Yeni Sayfa"
                        onClicked: appCtrl.whiteboard.addPage()
                    }
                    Button {
                        text: "Temizle"
                        onClicked: appCtrl.whiteboard.clearCurrentPage()
                    }
                }

                Rectangle { width: 1; height: 24; color: "#475569" }

                IconButton {
                    iconText: "✕"
                    tooltipText: "Kitaba Geri Dön"
                    normalColor: "#FFFFFF"
                    onClicked: appCtrl.sessionState.setWhiteboardOpen(false)
                }
            }
        }

        // Canvas Area
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            WhiteboardCanvasItem {
                id: wbCanvas
                anchors.fill: parent
                engine: appCtrl.whiteboard
                toolController: appCtrl.toolCtrl
            }

            // Route drawings on whiteboard
            MouseArea {
                anchors.fill: parent
                enabled: appCtrl.sessionState.activeToolAsInt >= 1 && appCtrl.sessionState.activeToolAsInt <= 7

                onPressed: function(mouse) {
                    var normX = mouse.x / width;
                    var normY = mouse.y / height;
                    var tool = appCtrl.sessionState.activeToolAsInt;
                    if (tool === 1 || tool === 2) {
                        appCtrl.toolCtrl.startStroke(1, tool, normX, normY, 1.0);
                    } else if (tool >= 4 && tool <= 7) {
                        appCtrl.toolCtrl.startShape(1, tool, normX, normY);
                    }
                }

                onPositionChanged: function(mouse) {
                    var normX = mouse.x / width;
                    var normY = mouse.y / height;
                    var tool = appCtrl.sessionState.activeToolAsInt;
                    if (tool === 1 || tool === 2) {
                        appCtrl.toolCtrl.addStrokePoint(normX, normY, 1.0);
                    } else if (tool >= 4 && tool <= 7) {
                        appCtrl.toolCtrl.updateShapeEnd(normX, normY);
                    }
                }

                onReleased: {
                    var tool = appCtrl.sessionState.activeToolAsInt;
                    if (tool === 1 || tool === 2) {
                        appCtrl.toolCtrl.endStroke();
                    } else if (tool >= 4 && tool <= 7) {
                        appCtrl.toolCtrl.endShape();
                    }
                    wbCanvas.update();
                }
            }
        }
    }
}
