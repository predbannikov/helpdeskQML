import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import "Global.js" as Glob
Page {
    title: qsTr("login")
    id: windowHome
    background: Rectangle {
        color: "#cc242421"
    }
    Component {
        id: fruitDelegate

        Column {
            Row {
                spacing: 10
                Text {
                    text: "#" + name
                    color: "#e6e6e6"
                    font.bold: true
                    font.pixelSize: Qt.application.font.pixelSize * 1.7

                }
                Text {
                    text: "id:" + cost
                    color: "#5555FF"
                    font.bold: true
                    font.pixelSize: Qt.application.font.pixelSize * 1.2

                }
                Text {
                    text: stat_id
                    color:  "open" == stat_id ? "#FF0000" : "#00FF00"
                    font.bold: true
                    font.pixelSize: Qt.application.font.pixelSize * 1.3
                }

            }
            Rectangle {
                color: "#1000FF00"
                height: 80
                width: windowHome.width
            }
        }

    }
    ListView {
        id: listView
        anchors.fill: parent
        model: fruitModel
        delegate: fruitDelegate


        onMovementStarted: {
            console.log("s")
        }

        onContentYChanged: {
            console.log("+")
//        if (bannerStart && (contentY <= params.bannerMargin))
//            showPullDownItem();
        }
//        onMovementStart: {
//        if (contentY==0) bannerStart = true;
//        else bannerStart = false;
//        }
        onVerticalVelocityChanged: {
            console.log("=")
        // Prevent triggering pulldown item when rebound from top boundary.
//        if (verticalVelocity>0) bannerStart = false;
        }
        onMovementEnded: {
            console.log("=====================")
//        bannerStart = false;
        // trigger timer to hide pull-down item
//        timer.restart();
        }






    }
    ListModel {
        id: fruitModel
    }
    Timer {
        id: timer
        interval: 5000;
        repeat: true;
        onTriggered: {
            var Params = {
                tickets: {
                    count: 100
                }
            }
            var jsonArray = JSON.stringify(Params)
            backEnd.getDataStor(jsonArray)

            backEnd.eventTimer()
        }
    }
    Component.onCompleted: {
        var Params = {
            tickets: {
                count: 100
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
            fruitModel.clear();
//            console.log(field)
            var obj = JSON.parse(field);

            for(var propt in obj) {
                fruitModel.append({"name": obj[propt].title, "cost": obj[propt].id, "stat_id": obj[propt].status_id})
            }
        }
    }
}
