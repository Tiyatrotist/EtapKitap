import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../panels"
import "../overlays"
import "../dialogs"
import "../components"

Rectangle {
    id: root

    color: "#E2E8F0"

    // Popover anchors and state
    property bool popoverVisible: false
    property string popoverMode: "pen"
    property real popoverX: 0
    property real popoverY: 0

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 1. Top Bar (auto-hides in Presentation Mode)
        TopBar {
            id: topBar
            Layout.fillWidth: true
            visible: !appCtrl.sessionState.presentationMode
        }

        // 2. Middle Work Area (Drawer + Book Canvas + ToolDock + Scratchpad)
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                spacing: 0

                // Left Navigation Drawer
                NavigationDrawer {
                    id: navDrawer
                    Layout.fillHeight: true
                    visible: appCtrl.sessionState.leftPanelOpen && !appCtrl.sessionState.presentationMode
                }

                // Main Book Viewport Area
                Item {
                    id: bookViewport
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    Flickable {
                        id: flickable
                        anchors.fill: parent
                        contentWidth: pageContainer.width * appCtrl.docCtrl.zoom
                        contentHeight: pageContainer.height * appCtrl.docCtrl.zoom
                        interactive: appCtrl.sessionState.activeToolAsInt === 0 // only pan when in pointer mode

                        Item {
                            id: pageContainer
                            anchors.centerIn: parent
                            width: appCtrl.docCtrl.viewMode === 1 && appCtrl.docCtrl.secondaryPage > 0 ?
                                   (singlePageArea.width * 2 + 20) : singlePageArea.width
                            height: singlePageArea.height
                            scale: appCtrl.docCtrl.zoom

                            // Single Page / Primary Page Area
                            Item {
                                id: singlePageArea
                                width: Math.min(bookViewport.width - 40, (bookViewport.height - 40) * (595.0 / 842.0))
                                height: width * (842.0 / 595.0)
                                x: appCtrl.docCtrl.viewMode === 1 && appCtrl.docCtrl.secondaryPage > 0 ? 0 :
                                   (pageContainer.width - width) / 2
                                y: (pageContainer.height - height) / 2

                                Rectangle {
                                    anchors.fill: parent
                                    color: "#FFFFFF"
                                    border.color: "#CBD5E1"
                                    border.width: 1
                                    radius: 4

                                    Image {
                                        id: primaryImage
                                        anchors.fill: parent
                                        fillMode: Image.PreserveAspectFit
                                        source: "image://page/" + appCtrl.docCtrl.currentPage +
                                                "?w=" + Math.round(width * appCtrl.docCtrl.zoom * 1.5) +
                                                "&h=" + Math.round(height * appCtrl.docCtrl.zoom * 1.5)
                                        asynchronous: true
                                        cache: false
                                    }

                                    // Hotspots layer
                                    HotspotOverlay {
                                        anchors.fill: parent
                                        pageNumber: appCtrl.docCtrl.currentPage
                                    }

                                    // Annotations & drawing layer
                                    DrawingCanvas {
                                        anchors.fill: parent
                                        pageNumber: appCtrl.docCtrl.currentPage
                                    }
                                }
                            }

                            // Secondary Page (for Dual Page mode)
                            Item {
                                id: secondaryPageArea
                                visible: appCtrl.docCtrl.viewMode === 1 && appCtrl.docCtrl.secondaryPage > 0
                                width: singlePageArea.width
                                height: singlePageArea.height
                                x: singlePageArea.width + 20
                                y: singlePageArea.y

                                Rectangle {
                                    anchors.fill: parent
                                    color: "#FFFFFF"
                                    border.color: "#CBD5E1"
                                    border.width: 1
                                    radius: 4

                                    Image {
                                        anchors.fill: parent
                                        fillMode: Image.PreserveAspectFit
                                        source: visible ? ("image://page/" + appCtrl.docCtrl.secondaryPage +
                                                "?w=" + Math.round(width * appCtrl.docCtrl.zoom * 1.5) +
                                                "&h=" + Math.round(height * appCtrl.docCtrl.zoom * 1.5)) : ""
                                        asynchronous: true
                                        cache: false
                                    }

                                    HotspotOverlay {
                                        anchors.fill: parent
                                        pageNumber: appCtrl.docCtrl.secondaryPage
                                    }

                                    DrawingCanvas {
                                        anchors.fill: parent
                                        pageNumber: appCtrl.docCtrl.secondaryPage
                                    }
                                }
                            }
                        }
                    }

                    // Smartboard Global Overlays on Top of Viewport
                    LaserOverlay {
                        anchors.fill: parent
                    }

                    SpotlightOverlay {
                        anchors.fill: parent
                    }

                    CurtainOverlay {
                        anchors.fill: parent
                    }
                }

                // Scratchpad Split Pane
                ScratchpadPane {
                    Layout.preferredWidth: parent.width * appCtrl.scratchpad.widthRatio
                    Layout.fillHeight: true
                    visible: appCtrl.sessionState.scratchpadOpen && !appCtrl.sessionState.presentationMode
                }

                // Right Tool Dock (hides in Presentation Mode)
                ToolDock {
                    id: toolDock
                    Layout.fillHeight: true
                    visible: !appCtrl.sessionState.presentationMode

                    onPenPropertyRequested: function(x, y) {
                        root.popoverMode = "pen";
                        root.popoverX = x - 230;
                        root.popoverY = Math.min(y, root.height - 230);
                        root.popoverVisible = !root.popoverVisible;
                    }

                    onEraserPropertyRequested: function(x, y) {
                        root.popoverMode = "eraser";
                        root.popoverX = x - 230;
                        root.popoverY = Math.min(y, root.height - 150);
                        root.popoverVisible = !root.popoverVisible;
                    }
                }
            }

            // Floating Minimal Toolbar (for Presentation Mode - Section 29)
            Rectangle {
                visible: appCtrl.sessionState.presentationMode
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 24
                width: floatingRow.implicitWidth + 24
                height: 56
                radius: 28
                color: "#1E293B"
                border.color: "#475569"
                border.width: 1

                RowLayout {
                    id: floatingRow
                    anchors.centerIn: parent
                    spacing: 8

                    IconButton {
                        iconText: "✏️"
                        tooltipText: "Kalem"
                        active: appCtrl.sessionState.activeToolAsInt === 1
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.sessionState.setActiveToolAsInt(1)
                    }

                    IconButton {
                        iconText: "🖍️"
                        tooltipText: "Fosforlu"
                        active: appCtrl.sessionState.activeToolAsInt === 2
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.sessionState.setActiveToolAsInt(2)
                    }

                    IconButton {
                        iconText: "🧹"
                        tooltipText: "Silgi"
                        active: appCtrl.sessionState.activeToolAsInt === 3
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.sessionState.setActiveToolAsInt(3)
                    }

                    IconButton {
                        iconText: "↩️"
                        tooltipText: "Geri Al"
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.toolCtrl.undoManager.undo()
                    }

                    IconButton {
                        iconText: "🔴"
                        tooltipText: "Lazer"
                        active: appCtrl.sessionState.activeToolAsInt === 8
                        normalColor: "#FFFFFF"
                        onClicked: {
                            appCtrl.sessionState.setActiveToolAsInt(8);
                            appCtrl.toolCtrl.laserPointer.setActive(true);
                        }
                    }

                    IconButton {
                        iconText: "◀"
                        tooltipText: "Önceki Sayfa"
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.docCtrl.previousPage()
                    }

                    Text {
                        text: appCtrl.docCtrl.currentPage + "/" + appCtrl.docCtrl.pageCount
                        color: "#FFFFFF"
                        font.bold: true
                    }

                    IconButton {
                        iconText: "▶"
                        tooltipText: "Sonraki Sayfa"
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.docCtrl.nextPage()
                    }

                    IconButton {
                        iconText: "⛶"
                        tooltipText: "Sunum Modundan Çık"
                        normalColor: "#FFFFFF"
                        onClicked: appCtrl.sessionState.setPresentationMode(false)
                    }
                }
            }
        }

        // 3. Bottom Page Bar (hides in Presentation Mode)
        BottomPageBar {
            Layout.fillWidth: true
            visible: !appCtrl.sessionState.presentationMode
        }
    }

    // Tool Property Popover
    ToolPropertyPopover {
        id: popover
        visible: root.popoverVisible
        x: root.popoverX
        y: root.popoverY
        mode: root.popoverMode
    }

    // Modals
    MediaModal {}
    ActivityModal {}
    WhiteboardModal {}
    FreezeOverlay {}
}
