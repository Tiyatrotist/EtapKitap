import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"

Rectangle {
    id: root

    height: 52
    color: "#FFFFFF"
    border.color: "#E1E6EB"
    border.width: 1

    RowLayout {
        anchors.centerIn: parent
        spacing: 12

        IconButton {
            iconText: "⇤"
            tooltipText: "İlk Sayfa"
            onClicked: appCtrl.docCtrl.firstPage()
        }

        IconButton {
            iconText: "◀"
            tooltipText: "Önceki Sayfa (Sol Ok / PgUp)"
            onClicked: appCtrl.docCtrl.previousPage()
        }

        RowLayout {
            spacing: 6
            TextInput {
                id: pageInput
                text: appCtrl.docCtrl.currentPage.toString()
                font.pixelSize: 15
                font.bold: true
                color: "#1E293B"
                horizontalAlignment: TextInput.AlignRight
                width: 38
                selectByMouse: true
                onAccepted: {
                    var p = parseInt(text);
                    if (!isNaN(p)) {
                        appCtrl.docCtrl.setCurrentPage(p);
                    }
                }
            }

            Text {
                text: "/ " + appCtrl.docCtrl.pageCount
                font.pixelSize: 15
                font.bold: true
                color: "#64748B"
            }
        }

        IconButton {
            iconText: "▶"
            tooltipText: "Sonraki Sayfa (Sağ Ok / PgDown)"
            onClicked: appCtrl.docCtrl.nextPage()
        }

        IconButton {
            iconText: "⇥"
            tooltipText: "Son Sayfa"
            onClicked: appCtrl.docCtrl.lastPage()
        }

        Rectangle {
            width: 1
            height: 26
            color: "#CBD5E1"
        }

        IconButton {
            iconText: "➖"
            tooltipText: "Uzaklaştır"
            onClicked: appCtrl.docCtrl.zoomOut()
        }

        Text {
            text: Math.round(appCtrl.docCtrl.zoom * 100) + "%"
            font.pixelSize: 13
            font.bold: true
            color: "#475569"
            width: 44
            horizontalAlignment: Text.AlignHCenter
        }

        IconButton {
            iconText: "➕"
            tooltipText: "Yakınlaştır"
            onClicked: appCtrl.docCtrl.zoomIn()
        }

        Rectangle {
            width: 1
            height: 26
            color: "#CBD5E1"
        }

        IconButton {
            iconText: appCtrl.docCtrl.viewMode === 1 ? "📖" : "📄"
            tooltipText: appCtrl.docCtrl.viewMode === 1 ? "Çift Sayfa Modu Aktif (Kitap)" : "Tek Sayfa Modu Aktif"
            active: appCtrl.docCtrl.viewMode === 1
            onClicked: {
                if (appCtrl.docCtrl.viewMode === 1) {
                    appCtrl.docCtrl.setViewMode(0); // Single
                } else {
                    appCtrl.docCtrl.setViewMode(1); // Dual
                }
            }
        }

        IconButton {
            iconText: "↔️"
            tooltipText: "Genişliğe Sığdır / Sayfaya Sığdır"
            onClicked: appCtrl.docCtrl.resetZoom()
        }
    }
}
