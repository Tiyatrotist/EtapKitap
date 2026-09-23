import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Rectangle {
    id: root

    width: 320
    color: "#FFFFFF"
    border.color: "#E1E6EB"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Tab Selector Header
        Rectangle {
            Layout.fillWidth: true
            height: 52
            color: "#F8FAFC"
            border.color: "#E2E8F0"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                spacing: 2

                Repeater {
                    model: [
                        { label: "Sayfalar", tab: 0 },
                        { label: "İçindekiler", tab: 1 },
                        { label: "Yer İmleri", tab: 2 },
                        { label: "Arama", tab: 3 },
                        { label: "Etkinlikler", tab: 4 }
                    ]

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: appCtrl.sessionState.activeNavTab === modelData.tab ? "#FFFFFF" : "transparent"

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: 3
                            color: "#1E5F8A"
                            visible: appCtrl.sessionState.activeNavTab === modelData.tab
                        }

                        Text {
                            anchors.centerIn: parent
                            text: modelData.label
                            font.pixelSize: 12
                            font.bold: appCtrl.sessionState.activeNavTab === modelData.tab
                            color: appCtrl.sessionState.activeNavTab === modelData.tab ? "#1E5F8A" : "#64748B"
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: appCtrl.sessionState.setActiveNavTab(modelData.tab)
                        }
                    }
                }
            }
        }

        // Tab Content Views
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: appCtrl.sessionState.activeNavTab

            // Tab 0: Pages / Thumbnails Grid
            ListView {
                id: thumbList
                model: appCtrl.docCtrl.pageModel
                clip: true
                spacing: 12
                topMargin: 12
                bottomMargin: 12

                delegate: Rectangle {
                    width: thumbList.width - 24
                    height: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    color: model.pageNumber === appCtrl.docCtrl.currentPage ? "#EFF6FF" : "#F8FAFC"
                    border.color: model.pageNumber === appCtrl.docCtrl.currentPage ? "#3B82F6" : "#E2E8F0"
                    border.width: model.pageNumber === appCtrl.docCtrl.currentPage ? 2 : 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12

                        // Thumbnail image
                        Image {
                            Layout.preferredWidth: 70
                            Layout.fillHeight: true
                            fillMode: Image.PreserveAspectFit
                            source: "image://page/" + model.pageNumber + "?w=140&h=200"
                            asynchronous: true
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Text {
                                text: "Sayfa " + model.pageNumber
                                font.pixelSize: 14
                                font.bold: true
                                color: "#1E293B"
                            }

                            Text {
                                text: model.isCover ? "Kapak Sayfası" : "Ders İçeriği"
                                font.pixelSize: 12
                                color: "#64748B"
                            }

                            Text {
                                visible: model.interactionCount > 0
                                text: "✨ " + model.interactionCount + " Etkileşim"
                                font.pixelSize: 11
                                font.bold: true
                                color: "#D97706"
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: appCtrl.docCtrl.setCurrentPage(model.pageNumber)
                    }
                }
            }

            // Tab 1: Table of Contents (Outline)
            ListView {
                model: appCtrl.outlineModel
                clip: true
                spacing: 6
                topMargin: 12

                delegate: Rectangle {
                    width: parent.width - 20
                    height: 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 6
                    color: mouseArea.containsMouse ? "#F1F5F9" : "transparent"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12

                        Text {
                            text: model.title
                            font.pixelSize: 13
                            font.bold: true
                            color: "#334155"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            height: 24
                            width: 32
                            radius: 12
                            color: "#E2E8F0"
                            Text {
                                anchors.centerIn: parent
                                text: model.page
                                font.pixelSize: 11
                                color: "#475569"
                            }
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: appCtrl.docCtrl.setCurrentPage(model.page)
                    }
                }
            }

            // Tab 2: Bookmarks
            Item {
                Text {
                    anchors.centerIn: parent
                    text: "Henüz yer imi eklenmedi.\nÜst bardaki ★ butonuna basarak ekleyebilirsiniz."
                    font.pixelSize: 13
                    color: "#94A3B8"
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // Tab 3: Search
            ColumnLayout {
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    Layout.rightMargin: 12
                    Layout.topMargin: 10
                    height: 42
                    radius: 8
                    color: "#F1F5F9"
                    border.color: "#CBD5E1"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 6

                        Text { text: "🔍"; font.pixelSize: 14; color: "#64748B" }

                        TextInput {
                            id: searchInput
                            Layout.fillWidth: true
                            font.pixelSize: 14
                            color: "#1E293B"
                            clip: true
                            onAccepted: appCtrl.searchEngine.search(appCtrl.docCtrl.currentDocument, text)
                        }

                        Text {
                            visible: searchInput.text.length > 0
                            text: "✕"
                            font.pixelSize: 12
                            color: "#94A3B8"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: { searchInput.text = ""; appCtrl.searchEngine.clear(); }
                            }
                        }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: appCtrl.searchEngine
                    clip: true
                    spacing: 8

                    delegate: Rectangle {
                        width: parent.width - 24
                        height: 64
                        anchors.horizontalCenter: parent.horizontalCenter
                        radius: 6
                        color: "#F8FAFC"
                        border.color: "#E2E8F0"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 2

                            Text {
                                text: model.title
                                font.pixelSize: 12
                                font.bold: true
                                color: "#1E5F8A"
                            }

                            Text {
                                text: model.snippet
                                font.pixelSize: 11
                                color: "#475569"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: appCtrl.docCtrl.setCurrentPage(model.page)
                        }
                    }
                }
            }

            // Tab 4: Activities & Interactions list
            ListView {
                clip: true
                spacing: 8
                topMargin: 10
                model: appCtrl.interactionRegistry.interactionsForPage(appCtrl.docCtrl.currentPage)

                delegate: Rectangle {
                    width: parent.width - 24
                    height: 54
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    color: "#FFFBEB"
                    border.color: "#FDE68A"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: modelData.typeString === "video" ? "🎬" :
                                  modelData.typeString === "audio" ? "🔊" :
                                  modelData.typeString === "activity" ? "📝" : "💡"
                            font.pixelSize: 18
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2

                            Text {
                                text: modelData.title
                                font.pixelSize: 12
                                font.bold: true
                                color: "#92400E"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Text {
                                text: modelData.teaser ? modelData.teaser : "Etkileşimi başlatmak için dokunun"
                                font.pixelSize: 10
                                color: "#B45309"
                                elide: Text.ElideRight
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: appCtrl.triggerHotspot(modelData.id)
                    }
                }
            }
        }
    }
}
