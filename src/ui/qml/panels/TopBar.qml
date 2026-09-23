import QtQuick
import QtQuick.Layouts
import "../components"

Rectangle {
    id: root

    height: 58
    color: "#FFFFFF"
    border.color: "#E1E6EB"
    border.width: 1

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 8

        IconButton {
            iconText: "☰"
            tooltipText: "Gezinme Paneli (Sayfalar, İçindekiler, Arama)"
            active: appCtrl.sessionState.leftPanelOpen
            onClicked: appCtrl.sessionState.toggleLeftPanel()
        }

        IconButton {
            iconText: "⌂"
            tooltipText: "Kütüphane / Ana Sayfa"
            onClicked: appCtrl.closeDocument()
        }

        // Title and Chapter info
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: appCtrl.docCtrl.title
                font.pixelSize: 16
                font.bold: true
                color: "#1E2A38"
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                text: "Pardus ETAP Etkileşimli Ders Kitabı"
                font.pixelSize: 11
                color: "#7F8C8D"
                elide: Text.ElideRight
            }
        }

        // Hotspot interaction indicator
        Rectangle {
            visible: appCtrl.interactionRegistry.interactionCountForPage(appCtrl.docCtrl.currentPage) > 0
            height: 32
            width: interactionLabel.implicitWidth + 20
            radius: 16
            color: "#FFF3E0"
            border.color: "#FFE0B2"

            Text {
                id: interactionLabel
                anchors.centerIn: parent
                text: "✨ " + appCtrl.interactionRegistry.interactionCountForPage(appCtrl.docCtrl.currentPage) + " Etkileşim"
                font.pixelSize: 12
                font.bold: true
                color: "#E65100"
            }
        }

        // Page Indicator
        Rectangle {
            height: 36
            width: 110
            radius: 18
            color: "#F1F5F9"

            Text {
                anchors.centerIn: parent
                text: appCtrl.docCtrl.currentPage + " / " + appCtrl.docCtrl.pageCount
                font.pixelSize: 14
                font.bold: true
                color: "#2C3E50"
            }
        }

        IconButton {
            iconText: "★"
            tooltipText: "Sayfayı Yer İmlerine Ekle"
            onClicked: appCtrl.addCurrentPageBookmark()
        }

        IconButton {
            iconText: "🔍"
            tooltipText: "Kitap İçinde Ara"
            active: appCtrl.sessionState.leftPanelOpen && appCtrl.sessionState.activeNavTab === 3
            onClicked: {
                appCtrl.sessionState.setActiveNavTab(3);
                appCtrl.sessionState.setLeftPanelOpen(true);
            }
        }

        IconButton {
            iconText: appCtrl.sessionState.teacherMode ? "🎓" : "👨‍🏫"
            tooltipText: appCtrl.sessionState.teacherMode ? "Öğretmen Modu Açık" : "Öğretmen Modu Kapalı"
            active: appCtrl.sessionState.teacherMode
            activeColor: "#D35400"
            onClicked: appCtrl.sessionState.toggleTeacherMode()
        }

        IconButton {
            iconText: "⛶"
            tooltipText: "Sunum / Tam Ekran Modu (F11)"
            active: appCtrl.sessionState.presentationMode
            onClicked: appCtrl.sessionState.togglePresentationMode()
        }
    }
}
