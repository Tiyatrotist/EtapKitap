import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Rectangle {
    id: root

    property bool active: appCtrl.sessionState.freezeOpen
    visible: active
    color: "#E2E8F0"

    // Snapshot image frozen in background
    Image {
        id: bgSnap
        anchors.fill: parent
        source: appCtrl.freeze.active ? ("image://page/" + appCtrl.docCtrl.currentPage + "?w=" + width + "&h=" + height) : ""
        fillMode: Image.PreserveAspectFit
    }

    // Top control banner
    Rectangle {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 16
        width: 360
        height: 48
        radius: 24
        color: "#1E293B"

        RowLayout {
            anchors.centerIn: parent
            spacing: 16

            Text {
                text: "❄️ Ekran Sabitlendi (Freeze)"
                color: "#93C5FD"
                font.bold: true
                font.pixelSize: 14
            }

            Button {
                text: "Temizle"
                onClicked: appCtrl.freeze.clear()
            }

            Button {
                text: "Dondurmayı Kapat"
                highlighted: true
                onClicked: appCtrl.sessionState.setFreezeOpen(false)
            }
        }
    }
}
