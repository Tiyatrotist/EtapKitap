import QtQuick

Item {
    id: root

    property int pageNumber: 1
    property var interactions: appCtrl.interactionRegistry.interactionsForPage(root.pageNumber)

    visible: appCtrl.sessionState.interactionsVisible

    Repeater {
        model: root.interactions

        Item {
            id: hotspotItem
            x: modelData.rect.x * root.width
            y: modelData.rect.y * root.height
            width: Math.max(48, modelData.rect.width * root.width)
            height: Math.max(48, modelData.rect.height * root.height)

            Rectangle {
                anchors.centerIn: parent
                width: 44
                height: 44
                radius: 22
                color: modelData.typeString === "video" ? "#E53935" :
                       modelData.typeString === "audio" ? "#1E88E5" :
                       modelData.typeString === "activity" ? "#FB8C00" : "#43A047"

                border.color: "#FFFFFF"
                border.width: 2.5

                // Subtle pulse ring
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width + 8
                    height: parent.height + 8
                    radius: width / 2
                    color: "transparent"
                    border.color: parent.color
                    border.width: 1.5
                    opacity: 0.6
                }

                Text {
                    anchors.centerIn: parent
                    text: modelData.typeString === "video" ? "▶" :
                          modelData.typeString === "audio" ? "🔊" :
                          modelData.typeString === "activity" ? "?" : "💡"
                    font.pixelSize: 18
                    color: "#FFFFFF"
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: appCtrl.triggerHotspot(modelData.id)
                }
            }
        }
    }
}
