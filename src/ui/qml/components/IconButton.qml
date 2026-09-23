import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string iconText: ""
    property string tooltipText: ""
    property bool active: false
    property color activeColor: "#1E5F8A"
    property color normalColor: "#2C3E50"
    property int badgeCount: 0
    property int iconSize: 24

    signal clicked()
    signal longPressed()

    implicitWidth: 54
    implicitHeight: 54

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 10
        color: root.active ? Qt.rgba(root.activeColor.r, root.activeColor.g, root.activeColor.b, 0.15) :
               mouseArea.pressed ? "#E2E8F0" :
               mouseArea.containsMouse ? "#EDF2F7" : "transparent"

        border.color: root.active ? root.activeColor : "transparent"
        border.width: root.active ? 2 : 0

        Behavior on color { ColorAnimation { duration: 120 } }
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: root.iconText
        font.pixelSize: root.iconSize
        font.family: "Segoe UI Symbol, Ubuntu, sans-serif"
        color: root.active ? root.activeColor : (mouseArea.pressed ? Qt.darker(root.normalColor, 1.2) : root.normalColor)
    }

    // Badge indicator (for interaction count or notifications)
    Rectangle {
        visible: root.badgeCount > 0
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 4
        anchors.rightMargin: 4
        width: 18
        height: 18
        radius: 9
        color: "#E67E22"

        Text {
            anchors.centerIn: parent
            text: root.badgeCount > 99 ? "99+" : root.badgeCount.toString()
            color: "#FFFFFF"
            font.pixelSize: 10
            font.bold: true
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
        onPressAndHold: root.longPressed()
    }

    ToolTip.visible: mouseArea.containsMouse && root.tooltipText.length > 0
    ToolTip.text: root.tooltipText
    ToolTip.delay: 400
}
