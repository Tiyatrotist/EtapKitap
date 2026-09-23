import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    visible: appCtrl.mediaModalOpen
    anchors.fill: parent

    // Dark backdrop
    Rectangle {
        anchors.fill: parent
        color: "#B0000000"
        MouseArea {
            anchors.fill: parent
            onClicked: appCtrl.setMediaModalOpen(false)
        }
    }

    // Modal Card
    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width - 60, appCtrl.isVideoMedia ? 850 : 500)
        height: Math.min(parent.height - 60, appCtrl.isVideoMedia ? 540 : 200)
        radius: 16
        color: "#1E222B"
        border.color: "#333D4F"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 12

            // Header Bar
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: (appCtrl.isVideoMedia ? "🎬 Video: " : "🔊 Ses: ") + appCtrl.mediaTitle
                    font.pixelSize: 17
                    font.bold: true
                    color: "#FFFFFF"
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }

                IconButton {
                    iconText: "✕"
                    tooltipText: "Kapat"
                    normalColor: "#CBD5E1"
                    onClicked: appCtrl.setMediaModalOpen(false)
                }
            }

            // Media Stage
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0F1218"
                radius: 10

                Text {
                    anchors.centerIn: parent
                    text: appCtrl.isVideoMedia ?
                          "▶ " + appCtrl.mediaTitle + "\n[Etap Medya Oynatıcı: " + appCtrl.mediaSourceUrl + "]" :
                          "🔊 Oynatılıyor: " + appCtrl.mediaTitle
                    font.pixelSize: 15
                    color: "#94A3B8"
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // Controls Bar
            RowLayout {
                Layout.fillWidth: true
                spacing: 14

                Button {
                    text: "▶ Oynat / Duraklat"
                    highlighted: true
                }

                Slider {
                    Layout.fillWidth: true
                    value: 0.25
                }

                Text {
                    text: "02:15 / 06:40"
                    font.pixelSize: 13
                    color: "#CBD5E1"
                }

                Button {
                    text: "Tam Ekran"
                }
            }
        }
    }
}
