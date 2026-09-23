import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property string mode: "pen" // "pen" or "eraser"

    width: 220
    height: mode === "pen" ? 210 : 130
    radius: 12
    color: "#FFFFFF"
    border.color: "#CBD5E1"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // PEN MODE
        ColumnLayout {
            visible: root.mode === "pen"
            spacing: 8
            Layout.fillWidth: true

            Text {
                text: "Kalem Rengi"
                font.pixelSize: 12
                font.bold: true
                color: "#475569"
            }

            RowLayout {
                spacing: 8
                Repeater {
                    model: ["#E53935", "#1E88E5", "#43A047", "#FB8C00", "#8E24AA", "#212121"]
                    Rectangle {
                        width: 26
                        height: 26
                        radius: 13
                        color: modelData
                        border.color: appCtrl.toolCtrl.penColor === modelData ? "#1E293B" : "transparent"
                        border.width: 2

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: appCtrl.toolCtrl.setPenColor(modelData)
                        }
                    }
                }
            }

            Text {
                text: "Kalınlık"
                font.pixelSize: 12
                font.bold: true
                color: "#475569"
                Layout.topMargin: 4
            }

            RowLayout {
                spacing: 8
                Repeater {
                    model: [
                        { label: "İnce", val: 0.002 },
                        { label: "Normal", val: 0.004 },
                        { label: "Kalın", val: 0.008 }
                    ]
                    Button {
                        text: modelData.label
                        highlighted: Math.abs(appCtrl.toolCtrl.penWidth - modelData.val) < 0.001
                        onClicked: appCtrl.toolCtrl.setPenWidth(modelData.val)
                    }
                }
            }
        }

        // ERASER MODE
        ColumnLayout {
            visible: root.mode === "eraser"
            spacing: 10
            Layout.fillWidth: true

            Text {
                text: "Silgi Modu"
                font.pixelSize: 12
                font.bold: true
                color: "#475569"
            }

            RowLayout {
                spacing: 8
                Button {
                    text: "Çizgi Sil (Stroke)"
                    highlighted: appCtrl.toolCtrl.eraserMode === 0
                    onClicked: appCtrl.toolCtrl.setEraserMode(0)
                }
                Button {
                    text: "Hassas Sil"
                    highlighted: appCtrl.toolCtrl.eraserMode === 1
                    onClicked: appCtrl.toolCtrl.setEraserMode(1)
                }
            }

            Button {
                text: "Sayfadaki Tüm Çizimleri Temizle"
                Layout.fillWidth: true
                onClicked: appCtrl.toolCtrl.clearPageAnnotations(appCtrl.docCtrl.currentPage)
            }
        }
    }
}
