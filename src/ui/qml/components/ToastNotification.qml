import QtQuick

Item {
    id: root

    property string message: ""
    property int durationMs: 2800

    function show(msg) {
        root.message = msg;
        toastRect.opacity = 1.0;
        hideTimer.restart();
    }

    implicitWidth: toastRect.width
    implicitHeight: toastRect.height

    Rectangle {
        id: toastRect
        anchors.centerIn: parent
        width: Math.min(600, label.implicitWidth + 40)
        height: 48
        radius: 24
        color: "#2C3E50"
        opacity: 0.0

        Behavior on opacity { NumberAnimation { duration: 250 } }

        Text {
            id: label
            anchors.centerIn: parent
            text: root.message
            color: "#FFFFFF"
            font.pixelSize: 14
            font.bold: true
            elide: Text.ElideRight
        }
    }

    Timer {
        id: hideTimer
        interval: root.durationMs
        onTimeout: toastRect.opacity = 0.0
    }
}
