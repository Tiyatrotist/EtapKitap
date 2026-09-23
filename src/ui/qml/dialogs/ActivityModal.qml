import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    visible: appCtrl.activityModel.isOpen
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#90000000"
        MouseArea {
            anchors.fill: parent
            onClicked: appCtrl.activityModel.close()
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width - 60, 680)
        height: Math.min(parent.height - 60, 520)
        radius: 16
        color: "#FFFFFF"
        border.color: "#CBD5E1"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 22
            spacing: 16

            // Header
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "📝 " + appCtrl.activityModel.title
                    font.pixelSize: 18
                    font.bold: true
                    color: "#1E293B"
                    Layout.fillWidth: true
                }

                IconButton {
                    iconText: "✕"
                    tooltipText: "Kapat"
                    onClicked: appCtrl.activityModel.close()
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#E2E8F0"
            }

            // Question Text
            Text {
                text: appCtrl.activityModel.questionText
                font.pixelSize: 15
                color: "#334155"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            // Options List
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: appCtrl.activityModel.options
                spacing: 10
                clip: true

                delegate: Rectangle {
                    width: parent.width
                    height: 52
                    radius: 10
                    color: appCtrl.activityModel.selectedIndex === index ? "#EFF6FF" : "#F8FAFC"
                    border.color: appCtrl.activityModel.selectedIndex === index ? "#3B82F6" : "#E2E8F0"
                    border.width: appCtrl.activityModel.selectedIndex === index ? 2 : 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 16
                        anchors.rightMargin: 16
                        spacing: 12

                        Rectangle {
                            width: 28
                            height: 28
                            radius: 14
                            color: appCtrl.activityModel.selectedIndex === index ? "#3B82F6" : "#E2E8F0"
                            Text {
                                anchors.centerIn: parent
                                text: String.fromCharCode(65 + index) // A, B, C, D
                                color: appCtrl.activityModel.selectedIndex === index ? "#FFFFFF" : "#475569"
                                font.bold: true
                                font.pixelSize: 13
                            }
                        }

                        Text {
                            text: modelData
                            font.pixelSize: 14
                            color: "#1E293B"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: appCtrl.activityModel.selectOption(index)
                    }
                }
            }

            // Feedback Banner
            Rectangle {
                visible: appCtrl.activityModel.showFeedback
                Layout.fillWidth: true
                height: 42
                radius: 8
                color: appCtrl.activityModel.isCorrect ? "#DEF7EC" : "#FDE8E8"
                border.color: appCtrl.activityModel.isCorrect ? "#31C48D" : "#F98080"

                Text {
                    anchors.centerIn: parent
                    text: appCtrl.activityModel.isCorrect ? "🎉 Tebrikler! Doğru cevap." : "❌ Tekrar deneyiniz."
                    font.bold: true
                    font.pixelSize: 14
                    color: appCtrl.activityModel.isCorrect ? "#03543F" : "#9B1C1C"
                }
            }

            // Teacher Hint or Solution Box
            Rectangle {
                visible: appCtrl.activityModel.showSolution || appCtrl.activityModel.hintText.length > 0
                Layout.fillWidth: true
                height: solLabel.implicitHeight + 20
                radius: 8
                color: "#FEF3C7"
                border.color: "#F59E0B"

                Text {
                    id: solLabel
                    anchors.centerIn: parent
                    width: parent.width - 24
                    wrapMode: Text.WordWrap
                    text: appCtrl.activityModel.showSolution ?
                          ("💡 Çözüm: " + appCtrl.activityModel.solutionText) :
                          ("ℹ️ İpucu: " + appCtrl.activityModel.hintText)
                    font.pixelSize: 13
                    color: "#92400E"
                }
            }

            // Action Buttons
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Button {
                    text: "Cevabı Kontrol Et"
                    highlighted: true
                    enabled: appCtrl.activityModel.isAnswered
                    onClicked: appCtrl.activityModel.submitAnswer()
                }

                Button {
                    text: "Sıfırla"
                    onClicked: appCtrl.activityModel.reset()
                }

                Item { Layout.fillWidth: true }

                // Teacher Mode pedagogical reveal controls
                Button {
                    text: "İpucu Göster"
                    visible: appCtrl.sessionState.teacherMode
                    onClicked: appCtrl.activityModel.revealHint()
                }

                Button {
                    text: "Çözümü Göster"
                    visible: appCtrl.sessionState.teacherMode
                    onClicked: appCtrl.activityModel.revealSolution()
                }
            }
        }
    }
}
