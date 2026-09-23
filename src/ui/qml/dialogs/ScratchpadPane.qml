import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import EtapKitap 1.0
import "../components"

Rectangle {
    id: root

    visible: appCtrl.sessionState.scratchpadOpen
    color: "#FFFFFF"
    border.color: "#CBD5E1"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true
            height: 48
            color: "#F8FAFC"
            border.color: "#E2E8F0"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text {
                    text: "📝 Çalışma Alanı (Scratchpad)"
                    font.pixelSize: 13
                    font.bold: true
                    color: "#334155"
                    Layout.fillWidth: true
                }

                Button {
                    text: "Temizle"
                    onClicked: appCtrl.scratchpad.clear()
                }

                IconButton {
                    iconText: "✕"
                    tooltipText: "Kapat"
                    onClicked: appCtrl.sessionState.setScratchpadOpen(false)
                }
            }
        }

        // Work Canvas
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#FFFFFF"

            // Grid background pattern
            Canvas {
                id: gridPat
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);
                    ctx.strokeStyle = "#F1F5F9";
                    ctx.lineWidth = 1;
                    for (var x = 0; x < width; x += 25) {
                        ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, height); ctx.stroke();
                    }
                    for (var y = 0; y < height; y += 25) {
                        ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(width, y); ctx.stroke();
                    }
                }
            }

            DrawingCanvasItem {
                id: scCanvas
                anchors.fill: parent
                pageNumber: 1
                annotationModel: appCtrl.scratchpad.annotations
                toolController: appCtrl.toolCtrl
            }

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
                    scCanvas.update();
                }
            }
        }
    }
}
