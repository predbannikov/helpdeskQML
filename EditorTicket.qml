import QtQuick 2.0
import QtQuick.Controls 2.3

Page {
    title: qsTr("Editor ticket")
    id: editorTicket
    anchors.margins: 15
    background: Rectangle {
        color: "#cc242421"
    }
    TextEdit {
        id: teEmail
        inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhEmailCharactersOnly
        text: Glob.email
        color: "#ebe5d1"
//        implicitHeight: 40
        Rectangle {
            anchors.bottom: parent.bottom
            color: "#444542"
            height: 1
        }
    }
    TextField {
        id: teTitle
//        Layout.alignment: Qt.AlignCenter
        placeholderText: qsTr("Title")
//        implicitWidth: columnItem.implicitWidth
//        implicitHeight: 40
        color: "#ebe5d1"
        background: Rectangle {
            anchors.bottom: parent.bottom
            color: "#444542"
            height: 1

        }
    }
}
