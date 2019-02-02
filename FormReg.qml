import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "Global.js" as Glob

Page {
    title: qsTr("submit form")
    id: pageSubmit
    background: Rectangle {
        color: "#cc242421"
    }


    ScrollView {
        contentWidth: -1
        anchors.fill: parent
        clip: true

        ColumnLayout {
            spacing: 10
            id: columnSubmit
            width: firstWindow.width
            Layout.alignment: Qt.AlignCenter
            TextField {
                Layout.alignment: Qt.AlignCenter
                id: teEmail
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhEmailCharactersOnly
                placeholderText: qsTr("Email")
                text: Glob.email
                color: "#ebe5d1"
                implicitWidth: columnItem.implicitWidth
                implicitHeight: 40
                background: Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#444542"
                    height: 1
                }
            }
            TextField {
                id: teTitle
                Layout.alignment: Qt.AlignCenter
                placeholderText: qsTr("Title")
                implicitWidth: columnItem.implicitWidth
                implicitHeight: 40
                color: "#ebe5d1"
                background: Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#444542"
                    height: 1

                }
            }
            TextField {
                id: teDescription
                Layout.alignment: Qt.AlignCenter
                placeholderText: qsTr("write this is problem")
                implicitWidth: columnItem.implicitWidth
                color: "#ebe5d1"
                background: Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#444542"
                    height: 1

                }
            }
            Button {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("Отправить")
                background: Rectangle {
                        color: "#fcbd00"
                        radius: 4
                        implicitWidth: columnItem.implicitWidth
                        implicitHeight: 40


                }
                onClicked: {
                    var Params = {
                        email: teEmail.text,
                        title: teTitle.text,
                        description: teDescription.text
                    }
                    var jsonArray = JSON.stringify(Params)
                    console.log(jsonArray)
                    backEnd.ticketCreate(jsonArray)
                    stackView.pop()
                }
            }

            Button {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("Отмена")
                background: Rectangle {
                        color: "#fcbd00"
                        radius: 4
                        implicitWidth: columnItem.implicitWidth
                        implicitHeight: 40


                }
                onClicked: {
                    stackView.pop()

                }
            }
        }
    }
}
