import QtQuick

Item {
    id: root

    property bool active: appCtrl.sessionState.activeToolAsInt === 8

    visible: active

    Canvas {
        id: laserCanvas
        anchors.fill: parent

        Connections {
            target: appCtrl.toolCtrl.laserPointer
            function onTrailUpdated() {
                laserCanvas.requestPaint();
            }
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var pts = appCtrl.toolCtrl.laserPointer.trailPoints();
            if (!pts || pts.length === 0) return;

            var laserColor = appCtrl.toolCtrl.laserPointer.color;

            // Draw trailing smooth glow
            ctx.lineCap = "round";
            ctx.lineJoin = "round";

            for (var i = 1; i < pts.length; ++i) {
                var alpha = (i / pts.length);
                ctx.beginPath();
                ctx.moveTo(pts[i - 1].pos.x, pts[i - 1].pos.y);
                ctx.lineTo(pts[i].pos.x, pts[i].pos.y);
                ctx.lineWidth = 6 * alpha;
                ctx.strokeStyle = "rgba(255, 60, 60, " + (alpha * 0.8) + ")";
                ctx.stroke();
            }

            // Draw current tip (glowing luminous dot)
            var lastPt = pts[pts.length - 1];
            ctx.beginPath();
            ctx.arc(lastPt.pos.x, lastPt.pos.y, 8, 0, Math.PI * 2);
            ctx.fillStyle = "rgba(255, 30, 30, 0.95)";
            ctx.shadowColor = "rgba(255, 0, 0, 1.0)";
            ctx.shadowBlur = 14;
            ctx.fill();

            // Inner bright white center
            ctx.beginPath();
            ctx.arc(lastPt.pos.x, lastPt.pos.y, 3.5, 0, Math.PI * 2);
            ctx.fillStyle = "#FFFFFF";
            ctx.fill();
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.BlankCursor
        onPositionChanged: function(mouse) {
            appCtrl.toolCtrl.laserPointer.addPoint(mouse.x, mouse.y);
        }
    }
}
