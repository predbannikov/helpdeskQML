import QtQuick 2.8
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import "."
import "Global.js" as Glob
ApplicationWindow {
    x: 1400
    y: 1200
    width: 360
    height: 640
    visible: true
    color: "#cc242421"
    title: "Dialogs Demo"
    id: firstWindow

//    ScrollView {
//        contentWidth: -1
//        anchors.fill: parent
//        clip: true

        ColumnLayout {
            spacing: 10
            id: columnItem
            width: firstWindow.width
            Layout.alignment: Qt.AlignCenter
            Image {
                scale: 0.5
                Layout.alignment: Qt.AlignCenter
//                anchors.top: parent.top
                fillMode: Image.PreserveAspectFit
                source: "design.ico"
            }
            Text {
                Layout.alignment: Qt.AlignCenter
                text: "Welcome"
                color: "#e6e6e6"
                font.family: "Times New Roman"
                font.italic: true
                font.bold: true
                font.pixelSize: Qt.application.font.pixelSize * 2
            }
            Text {
                id: txtAppName
                Layout.alignment: Qt.AlignCenter
                text: "APP NAME"
                color: "#e6e6e6"
                font.family: "Arial"
                font.bold: true
                font.pixelSize: Qt.application.font.pixelSize * 3.2
            }
            TextField {
                Layout.alignment: Qt.AlignCenter
                id: teLogin
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhEmailCharactersOnly
                placeholderText: "EMAIL"
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
                id: tePassword
                Layout.alignment: Qt.AlignCenter
                placeholderText: qsTr("PASSWORD")
                echoMode: TextInput.Password
                implicitWidth: columnItem.implicitWidth
                implicitHeight: 40
                color: "#ebe5d1"
                background: Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#444542"
                    height: 1

                }

            }
            Button {
                Layout.alignment: Qt.AlignCenter
//                anchors.bottom: parent.anchors.bottom
                text: "Sign in"
                background: Rectangle {
                        color: "#fcbd00"
                        radius: 4
                        implicitWidth: columnItem.implicitWidth
                        implicitHeight: 40


                }
                onClicked: {
                    var params = {
                        email: teLogin.text,
                        password: tePassword.text,
                        path: Glob.path
                    }
                    var jsonArray = JSON.stringify(params)
//                    console.log(jsonArray)
                    backEnd.login(jsonArray)
                }
            }


        }
//    }
    Connections {
        target: backEnd
        onSendUserData: {
//            console.log("qml signal userdata" + jsonArray)
            Glob.setJDataUser(jsonArray)

            loader.source = "HomeForm.qml"
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
//        anchors.margins: 15
//        source: HomeForm
    }


}

