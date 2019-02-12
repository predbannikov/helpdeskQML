import QtQuick 2.11
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import "Global.js" as Glob

Page {
    title: qsTr("login")
    id: windowHome
//    anchors.margins: 15
    background: Rectangle {
        color: "#cc242421"
    }
    property int  stepIndex: 35

    Menu {
        id: menu
        property int id_index: 1
        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 500 }
        }
        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 500 }
        }
        MenuItem {
            height: 90
            id: menuItem
            property int  indexItemId: 0

            Rectangle{
                anchors.fill: parent
                color: "#5000FF00"
                radius: 5

                Column{

                    MenuItem {
                        height: 30
                        text: "Открыто"
                        onClicked: {
                            var Params = { status_id:  "open" }
                            var jsonArray = JSON.stringify(Params);
                            backEnd.updateStatusTicket(jsonArray, menuItem.indexItemId)
                            menu.close()
                        }
                    }
                    MenuItem {
                        height: 30
                        text: "В процессе"
                        onClicked: {
                            var Params = { status_id:  "process" }
                            var jsonArray = JSON.stringify(Params);
                            backEnd.updateStatusTicket(jsonArray, menuItem.indexItemId)
                            menu.close()
                        }
                    }
                    MenuItem {
                        height: 30
                        text: "Выполнено"
                        onClicked: {
                            var Params = { status_id:  "closed" }
                            var jsonArray = JSON.stringify(Params);
                            backEnd.updateStatusTicket(jsonArray, menuItem.indexItemId)
                            menu.close()
                        }
                    }
                }
            }


        }
    }

    Component {
        id: fruitDelegate


            Rectangle {
                color: "#09245c13"
                height: textName.height
                width: windowHome.width
                id: squadRos
//                anchors.margins: 15
                Text {
//                    x: 15
                    id: textName
                    text: "#" + name
                    color: "#e6e6e6"
                    font.pixelSize: Qt.application.font.pixelSize * 1.7
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            var objItem = fruitModel.get(index)
                            Glob.setItem(objItem.id_ticket, objItem.name, objItem.stat_id);
                            stackView.push("EditorTicket.qml")
                        }
                    }
                }
                Text {
                    visible: false
                    anchors.left: textName.right
                    text: "id:" + id_ticket
                    color: "#5555FF"
                    font.bold: true
                    font.pixelSize: Qt.application.font.pixelSize * 1.2

                }
                Text {
                    anchors.right: parent.right
//                    anchors.margins: 15
                    text: stat_id
//                    color:  "open" == stat_id ? "#FF0000" : "#00FF00"
                    color:  if("open" == stat_id)
                                return "#FF0000";
                            else  if("process" == stat_id)
                                return ("#ff9900");
                            else
                                return "#00FF00";
                    font.bold: true
                    font.pixelSize: Qt.application.font.pixelSize * 1.3
                    MouseArea {
                        anchors.fill: parent
                        onPressAndHold: {
                            var objItem = fruitModel.get(index)
                            menuItem.indexItemId = objItem.id_ticket
                            menu.popup()

                        }
                        onClicked:  {
//                            popup.open()
//                            menu.open()
                        }
                    }
                }
                Rectangle {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#444542"
                        height: 1
                        width: parent.width / 1.1
                    }
            }
    }
    ListView {
        id: listView
        anchors.fill: parent
        model: fruitModel
        delegate: fruitDelegate
        spacing: 15

        property bool updateState: false

//        states: [
//            State { name: "state1"; when: listView.updateState },
//            State { name: "state2"; when: listView.updateState }
//        ]


        onMovementStarted: {
        }

        onContentYChanged: {
            var posEvent = contentHeight - availableHeight

            if(posEvent < Math.abs(childrenRect.y)) {
                if(!listView.updateState) {
                    listView.updateState = true
                    Glob.indexStart += stepIndex
                    Glob.indexEnd += stepIndex
//                    console.log(Glob.indexStart)
//                    console.log(Glob.indexEnd)
                    console.log("+++", listView.updateState)
                    var Params = {
                        sortABC: {
                            startIndex: Glob.indexStart,
                            endIndex: Glob.indexEnd
                        }
                    }
                    var jsonArray = JSON.stringify(Params)
                    backEnd.getDataStor(jsonArray)
                }
            }
        }
        onVerticalVelocityChanged: {
        }
        onMovementEnded: {
//            console.log(contentHeight - availableHeight )
//            console.log(Math.abs(childrenRect.y) )
            console.log("=====================")
        }

    }
    ListModel {
        id: fruitModel
    }


    Timer {
        id: timer
        interval: 60000;                                        // Таймаут обновления
        repeat: true;
        onTriggered: {
            backEnd.eventTimer()
        }
    }

    Component.onCompleted: {
        Glob.indexStart = 0
        Glob.indexEnd = stepIndex
        var Params = {
            sortABC: {
                startIndex: Glob.indexStart,
                endIndex: Glob.indexEnd
            }
        }
        var jsonArray = JSON.stringify(Params)
        backEnd.getDataStor(jsonArray)
        timer.running = true
    }


    Button {
        id: downButton
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        implicitWidth: windowHome.width < 350 ? windowHome.width - 70 : 250
        anchors.margins:  15

        text: qsTr("Создать")
        background: Rectangle {
                color: "#fcbd00"
                radius: 4
                implicitHeight: 40
        }
        onClicked: {
            stackView.push("FormReg.qml")
        }
    }



    Connections {
        target: backEnd
        onSendField: {
            listView.updateState = false;
            var jarr = JSON.parse(field);
            for(var i = 0; i < jarr.length; i++) {
                fruitModel.append({"name": jarr[i].title, "id_ticket": jarr[i].id, "stat_id": jarr[i].status_id})
            }
        }
        onSendCustomData: {
            var jarr = JSON.parse(jarray);
            for(var i = 0; i < fruitModel.count; i++) {
                var obj = fruitModel.get(i);

                for(var j = 0; j < jarr.length; j++) {
                    if(obj.id_ticket === jarr[j].id) {
                        console.log(obj.id_ticket, obj.name, obj.stat_id);
                        fruitModel.set(i, {"name": jarr[j].title, "id_ticket": jarr[j].id, "stat_id": jarr[j].status_id});
                        fruitModel.move(i, 0, 1);
                    }
                }
            }

        }
    }
}
