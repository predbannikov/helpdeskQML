import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import "Global.js" as Glob

Page {
//    width: 600
//    height: 400
    signal signalHome

    title: qsTr("Home")
    id: windowHome
//    color: "#cc242421"
    background: Rectangle {
//        color: "#cc242421"
        color: "#242421"
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight
        id: toolBar
        background: Rectangle {
            implicitHeight: 45
            color: "#cc242421"

            Rectangle {
                width: parent.width
                height: 2
                anchors.bottom: parent.bottom
                color: "#cc242421"
                border.color: "#444542"
            }
        }


        ToolButton {
            implicitHeight: 40
            id: toolButton
            Text {
                text: "\u25C0"
                font: toolButton.font
                color: toolButton.down ? "#cc242421" : "#b2b3b1"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 40
                color: Qt.darker("#b2b3b1", toolButton.enabled && (toolButton.checked || toolButton.highlighted) ? 1.5 : 1.0)
                visible: toolButton.down || (toolButton.enabled && (toolButton.checked || toolButton.highlighted))
            }
            font.pixelSize: Qt.application.font.pixelSize * 3.2
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    teLogin.text = Glob.email
                    tePassword.text = Glob.password
                    loader.setSource("")
                    backEnd.logout()
                }
            }
        }
        Label {
            text: stackView.currentItem.title
            color: "#fabb00"
            anchors.centerIn: parent
            font.pixelSize: Qt.application.font.pixelSize * 1.9
        }

    }
    footer: TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        Text {
            id: textRespons
        }
    }



//    Connections {
//        target: backEnd
//        onSendRespons: {                // Пока не используем
//            textRespons.text = str
//            console.log(str)
//        }

//    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: FormLogin {}
    }
}
