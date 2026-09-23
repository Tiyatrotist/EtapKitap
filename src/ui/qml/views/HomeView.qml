import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components"

Rectangle {
    id: root

    color: "#F8FAFC"

    FileDialog {
        id: fileDialog
        title: "Kitap veya PDF Aç"
        nameFilters: ["Tüm Desteklenen Dosyalar (*.akbook *.pdf)", "Etkileşimli Kitap (*.akbook)", "PDF Dokümanı (*.pdf)"]
        onAccepted: {
            var path = selectedFile.toString();
            // Strip file:// prefix
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            } else if (path.startsWith("file://")) {
                path = path.substring(7);
            }
            appCtrl.openDocument(path);
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 28

        // Top Brand Header
        RowLayout {
            spacing: 16

            Rectangle {
                width: 60
                height: 60
                radius: 16
                color: "#1E5F8A"

                Text {
                    anchors.centerIn: parent
                    text: "📖"
                    font.pixelSize: 32
                }
            }

            ColumnLayout {
                spacing: 2
                Text {
                    text: "EtapKitap"
                    font.pixelSize: 28
                    font.bold: true
                    color: "#0F172A"
                }
                Text {
                    text: "Pardus ETAP Etkileşimli Dijital Ders Kitabı ve Akıllı Tahta Çalışma Ortamı"
                    font.pixelSize: 14
                    color: "#64748B"
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "📂 Kitap / PDF Aç"
                highlighted: true
                font.pixelSize: 15
                Layout.preferredHeight: 48
                onClicked: fileDialog.open()
            }
        }

        // Continue Reading Card
        Rectangle {
            visible: appCtrl.libraryModel.hasRecentDocuments
            Layout.fillWidth: true
            height: 120
            radius: 16
            color: "#EFF6FF"
            border.color: "#BFDBFE"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Rectangle {
                    width: 50
                    height: 50
                    radius: 25
                    color: "#3B82F6"
                    Text {
                        anchors.centerIn: parent
                        text: "🔖"
                        font.pixelSize: 24
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Kaldığınız Yerden Devam Edin"
                        font.pixelSize: 13
                        font.bold: true
                        color: "#2563EB"
                    }

                    Text {
                        text: appCtrl.libraryModel.continueTitle
                        font.pixelSize: 20
                        font.bold: true
                        color: "#1E293B"
                    }

                    Text {
                        text: "Son çalışılan: Sayfa " + appCtrl.libraryModel.continuePage
                        font.pixelSize: 13
                        color: "#64748B"
                    }
                }

                Button {
                    text: "Devam Et ▶"
                    highlighted: true
                    font.pixelSize: 15
                    Layout.preferredHeight: 46
                    onClicked: appCtrl.openDocument(appCtrl.libraryModel.continueFilePath)
                }
            }
        }

        // Recent Books Section Title
        Text {
            text: "Son Kullanılan Ders Kitapları"
            font.pixelSize: 18
            font.bold: true
            color: "#1E293B"
            Layout.topMargin: 8
        }

        // Recent Books Grid
        GridView {
            id: recentsGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: 260
            cellHeight: 140
            model: appCtrl.libraryModel
            clip: true

            delegate: Rectangle {
                width: 240
                height: 120
                radius: 12
                color: "#FFFFFF"
                border.color: mouseArea.containsMouse ? "#3B82F6" : "#E2E8F0"
                border.width: mouseArea.containsMouse ? 2 : 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 14

                    Rectangle {
                        width: 54
                        height: 74
                        radius: 6
                        color: "#F1F5F9"
                        border.color: "#CBD5E1"
                        Text {
                            anchors.centerIn: parent
                            text: model.format === "akbook" ? "✨\nKİTAP" : "📄\nPDF"
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 11
                            font.bold: true
                            color: "#475569"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: model.title
                            font.pixelSize: 15
                            font.bold: true
                            color: "#1E293B"
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Text {
                            text: "Kaldığınız yer: Sayfa " + model.lastPage
                            font.pixelSize: 12
                            color: "#64748B"
                        }

                        Text {
                            text: model.pageCount + " sayfa"
                            font.pixelSize: 11
                            color: "#94A3B8"
                        }
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: appCtrl.openDocument(model.filePath)
                }
            }
        }
    }
}
