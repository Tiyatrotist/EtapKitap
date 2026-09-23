import QtQuick

Item {
    id: root

    property bool active: appCtrl.toolCtrl.curtainTool.active
    visible: active

    // Direction 0: TopToBottom, 1: BottomToTop, 2: LeftToRight, 3: RightToLeft
    readonly property int dir: appCtrl.toolCtrl.curtainTool.direction

    // Curtain body
    Rectangle {
        id: curtainBody
        color: Qt.rgba(0.08, 0.10, 0.15, appCtrl.toolCtrl.curtainTool.opacity)

        x: root.dir === 3 ? (root.width * (1.0 - appCtrl.toolCtrl.curtainTool.progress)) : 0
        y: root.dir === 1 ? (root.height * (1.0 - appCtrl.toolCtrl.curtainTool.progress)) : 0
        width: (root.dir === 2 || root.dir === 3) ? (root.width * appCtrl.toolCtrl.curtainTool.progress) : root.width
        height: (root.dir === 0 || root.dir === 1) ? (root.height * appCtrl.toolCtrl.curtainTool.progress) : root.height

        // Draggable grip handle
        Rectangle {
            id: handle
            width: (root.dir === 0 || root.dir === 1) ? 140 : 28
            height: (root.dir === 0 || root.dir === 1) ? 28 : 140
            radius: 14
            color: "#E67E22"

            anchors.horizontalCenter: (root.dir === 0 || root.dir === 1) ? parent.horizontalCenter : undefined
            anchors.verticalCenter: (root.dir === 2 || root.dir === 3) ? parent.verticalCenter : undefined
            anchors.bottom: root.dir === 0 ? parent.bottom : undefined
            anchors.top: root.dir === 1 ? parent.top : undefined
            anchors.right: root.dir === 2 ? parent.right : undefined
            anchors.left: root.dir === 3 ? parent.left : undefined

            Text {
                anchors.centerIn: parent
                text: (root.dir === 0 || root.dir === 1) ? "═" : "║"
                color: "#FFFFFF"
                font.bold: true
                font.pixelSize: 18
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: (root.dir === 0 || root.dir === 1) ? Qt.SizeVerCursor : Qt.SizeHorCursor
                property real startCoord: 0

                onPressed: function(mouse) {
                    startCoord = (root.dir === 0 || root.dir === 1) ? mouse.y : mouse.x;
                }

                onPositionChanged: function(mouse) {
                    if (root.dir === 0) { // Top to bottom
                        var p = (curtainBody.height + (mouse.y - startCoord)) / root.height;
                        appCtrl.toolCtrl.curtainTool.setProgress(p);
                    } else if (root.dir === 1) { // Bottom to top
                        var p = (root.height - (curtainBody.y + mouse.y)) / root.height;
                        appCtrl.toolCtrl.curtainTool.setProgress(p);
                    } else if (root.dir === 2) { // Left to right
                        var p = (curtainBody.width + (mouse.x - startCoord)) / root.width;
                        appCtrl.toolCtrl.curtainTool.setProgress(p);
                    } else if (root.dir === 3) { // Right to left
                        var p = (root.width - (curtainBody.x + mouse.x)) / root.width;
                        appCtrl.toolCtrl.curtainTool.setProgress(p);
                    }
                }
            }
        }
    }

    // Close button
    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        width: 40
        height: 40
        radius: 20
        color: "#C0392B"

        Text {
            anchors.centerIn: parent
            text: "✕"
            color: "#FFFFFF"
            font.bold: true
            font.pixelSize: 16
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: appCtrl.toolCtrl.curtainTool.setActive(false)
        }
    }
}
