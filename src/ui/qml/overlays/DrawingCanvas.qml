import QtQuick
import EtapKitap 1.0

Item {
    id: root

    property int pageNumber: 1
    property bool interactive: appCtrl.sessionState.activeToolAsInt >= 1 && appCtrl.sessionState.activeToolAsInt <= 7

    DrawingCanvasItem {
        id: canvasItem
        anchors.fill: parent
        pageNumber: root.pageNumber
        annotationModel: appCtrl.annotationModel
        toolController: appCtrl.toolCtrl
    }

    // Touch and mouse drawing input router
    MouseArea {
        id: drawMouseArea
        anchors.fill: parent
        enabled: root.interactive
        hoverEnabled: false
        cursorShape: appCtrl.sessionState.activeToolAsInt === 3 ? Qt.CrossCursor : Qt.ArrowCursor

        onPressed: function(mouse) {
            var normX = mouse.x / root.width;
            var normY = mouse.y / root.height;
            var tool = appCtrl.sessionState.activeToolAsInt;

            if (tool === 1 || tool === 2) {
                // Pen / Highlighter
                appCtrl.toolCtrl.startStroke(root.pageNumber, tool, normX, normY, 1.0);
            } else if (tool >= 4 && tool <= 7) {
                // Shapes
                appCtrl.toolCtrl.startShape(root.pageNumber, tool, normX, normY);
            } else if (tool === 3) {
                // Eraser
                appCtrl.toolCtrl.eraseAt(root.pageNumber, normX, normY, 0.025);
            }
        }

        onPositionChanged: function(mouse) {
            var normX = mouse.x / root.width;
            var normY = mouse.y / root.height;
            var tool = appCtrl.sessionState.activeToolAsInt;

            if (tool === 1 || tool === 2) {
                appCtrl.toolCtrl.addStrokePoint(normX, normY, 1.0);
            } else if (tool >= 4 && tool <= 7) {
                appCtrl.toolCtrl.updateShapeEnd(normX, normY);
            } else if (tool === 3) {
                appCtrl.toolCtrl.eraseAt(root.pageNumber, normX, normY, 0.025);
            }
        }

        onReleased: {
            var tool = appCtrl.sessionState.activeToolAsInt;
            if (tool === 1 || tool === 2) {
                appCtrl.toolCtrl.endStroke();
            } else if (tool >= 4 && tool <= 7) {
                appCtrl.toolCtrl.endShape();
            }
        }
    }
}
