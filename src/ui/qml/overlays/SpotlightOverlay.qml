import QtQuick
import QtQuick.Controls

Item {
    id: root

    property bool active: appCtrl.toolCtrl.spotlightTool.active

    visible: active

    Canvas {
        id: spotCanvas
        anchors.fill: parent

        Connections {
            target: appCtrl.toolCtrl.spotlightTool
            function onPositionChanged() { spotCanvas.requestPaint(); }
            function onSizeChanged() { spotCanvas.requestPaint(); }
            function onShapeChanged() { spotCanvas.requestPaint(); }
            function onOpacityChanged() { spotCanvas.requestPaint(); }
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            // Fill entire canvas with dark dimming mask
            var op = appCtrl.toolCtrl.spotlightTool.dimOpacity;
            ctx.fillStyle = "rgba(10, 15, 25, " + op + ")";
            ctx.fillRect(0, 0, width, height);

            // Cut out spotlight hole
            ctx.globalCompositeOperation = "destination-out";

            var cx = appCtrl.toolCtrl.spotlightTool.centerX;
            var cy = appCtrl.toolCtrl.spotlightTool.centerY;

            if (appCtrl.toolCtrl.spotlightTool.isCircle) {
                var r = appCtrl.toolCtrl.spotlightTool.radius;
                ctx.beginPath();
                ctx.arc(cx, cy, r, 0, Math.PI * 2);
                ctx.fill();
            } else {
                var rw = appCtrl.toolCtrl.spotlightTool.rectWidth;
                var rh = appCtrl.toolCtrl.spotlightTool.rectHeight;
                ctx.fillRect(cx - rw / 2, cy - rh / 2, rw, rh);
            }

            ctx.globalCompositeOperation = "source-over";
        }
    }

    // Draggable center area
    MouseArea {
        anchors.fill: parent
        drag.target: dragProxy
        drag.axis: Drag.XAndYAxis

        Item {
            id: dragProxy
            x: appCtrl.toolCtrl.spotlightTool.centerX
            y: appCtrl.toolCtrl.spotlightTool.centerY
            onXChanged: appCtrl.toolCtrl.spotlightTool.setCenterX(x)
            onYChanged: appCtrl.toolCtrl.spotlightTool.setCenterY(y)
        }
    }

    // Close button
    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 20
        width: 44
        height: 44
        radius: 22
        color: "#C0392B"

        Text {
            anchors.centerIn: parent
            text: "✕"
            color: "#FFFFFF"
            font.pixelSize: 18
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: appCtrl.toolCtrl.spotlightTool.setActive(false)
        }
    }
}
