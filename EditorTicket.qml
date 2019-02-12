import QtQuick 2.0
import QtQuick.Controls 2.3
import "Global.js" as Glob

Page {

    title: qsTr("id " + Glob.curIdToEdit)
    id: editorTicket

    background: Rectangle {
        color: "#cc242421"
    }

    ToolTip {
        id: dialog
        modal: false
        delay: 0
        timeout: 1500
        opacity: 0.1
        background: Rectangle{
            color: "#E7BE6F"
            opacity: 0.5
            radius: 5
        }
        Text {
            anchors.fill: parent
            wrapMode: Text.WordWrap
            font.pixelSize: 24
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 500 }
        }
        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 500 }
        }
    }

    Column {
        spacing: 15
        Rectangle {
            width: editorTicket.width
            height: titleEdit.height + rowTitle.height + rowTitle.anchors.margins * 2

            opacity: 0.7
            color: "#352421"
            radius: 5

            TextField {
                id: titleEdit
                anchors.right: parent.right
                anchors.left: parent.left
                text: Glob.itemListView.name
                color: "#FF9900"
                background: Rectangle {
                    anchors.fill: parent
                    color: "#494949"
                    radius: 5
                }
                placeholderText: {
                    color: "#FF9900"
                    text == '' ? qsTr("Required title") : ""
                }
            }

            Row {
                id: rowTitle
                anchors.top: titleEdit.bottom
                anchors.left: parent.left
                anchors.margins: 5
                spacing: 10
                height: 35
                Button {
                    height: parent.height
                    text: qsTr("Сохранить")

                    background: Rectangle {
                        color: "#fcbd00"
                        radius: 4
                    }
                    onClicked: {
                        if(titleEdit.text == "") {
                            dialog.text = "The text should not be empty"
                            dialog.open();
                        } else {
                            var Params = {
                                title:  titleEdit.text ,
                                status_id: comboItem.currentText,
                                            // разобраься с ошибкой
                                            // "Access to the field {ticket_lock} is denied"
//                                ticket_lock: checkBox.isToggled
                            }
                            var jsonArray = JSON.stringify(Params);
                            console.log(jsonArray, Glob.itemListView.id);
                            backEnd.updateStatusTicket(jsonArray, Glob.itemListView.id)
                        }
                    }
                }
                Rectangle           // Checkbox
                {
                    id: checkBox
                    property bool isToggled: true
                    width: childrenRect.width
                    height: rowTitle.height
                    color: "transparent"
                    Row
                    {
                        spacing: 15
                        height: parent.height

                        Text {                          // Icon
                            verticalAlignment: Text.AlignVCenter
                            color: "#FF9900"
                            font.pixelSize: 30
                            text: checkBox.isToggled ? "\uf046" : "\uf096"
                            width: 15
                        }
                        Text
                        {
                            verticalAlignment: Text.AlignVCenter
                            height: parent.height
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
                ComboBox {
                    width: 90
                    height: rowTitle.height
                    id: comboItem
                    delegate: ItemDelegate {
                        contentItem: Text {
                            text: modelData
                            color: "#242421"
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            width: comboItem.width
                            color: "#fcbd00"
                            border.color: "#242421"
                        }
                    }
                    background: Rectangle {
                        color: "#fcbd00"
                        radius: 5
                        width: comboItem.width
                    }

                    model: ListModel {
                        ListElement { text: "open" }
                        ListElement {text: "process" }
                        ListElement {text: "closed" }
                    }
                    Component.onCompleted: {
                        switch(Glob.itemListView.status_id) {
                        case "open":
                            comboItem.currentIndex = 0;
                            break;
                        case "process":
                            comboItem.currentIndex = 1;
                            break;
                        case "closed":
                            comboItem.currentIndex = 2;
                            break;
                        }
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
