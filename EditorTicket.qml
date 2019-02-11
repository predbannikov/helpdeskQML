import QtQuick 2.0
import QtQuick.Controls 2.3
import "Global.js" as Glob

Page {

    title: qsTr("id " + Glob.curIdToEdit)
    id: editorTicket

    background: Rectangle {
        color: "#cc242421"
    }
    Column {
        spacing: 15
        Rectangle {
            width: editorTicket.width
            height: titleEdit.height + checkBox.height
            opacity: 0.7
            color: "#352421"
            radius: 5
            TextField {
                id: titleEdit
                anchors.right: parent.right
                anchors.left: parent.left
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhEmailCharactersOnly
                background: Rectangle {
                    anchors.fill: parent
                    color: "#494949"
                    radius: 5
//                    height: 1

                }
                placeholderText: {
                    color: "#FF9900"
                    text == '' ? qsTr("Required title") : ""
                }
                text: "textedit"

                color: "#FF9900"
            }
            Rectangle
            {
                id: checkBox
                anchors.top: titleEdit.bottom
                anchors.left: parent.left
                property bool isToggled: true
                width: childrenRect.width
                height: childrenRect.height
                color: "transparent"
                Row
                {
                    spacing: 15
                    Text {                          // Icon
                        color: "#FF9900"
                        font.pixelSize: 30
                        text: checkBox.isToggled ? "\uf046" : "\uf096"
                        width: 15
                    }
                    Text
                    {
                        verticalAlignment: Text.AlignVCenter
                        height: 35
                        color: "#FF9900"
                        text: "Lock"
                        font.pixelSize: 15
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        checkBox.isToggled = !checkBox.isToggled
                    }
                }
            }
        }
        Rectangle {
            width: editorTicket.width
            height: 50
            opacity: 0.5
            color: "red"
        }
    }
}
