import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"

Rectangle {
    id: rWarlock

    property var warlock: ({})

    LargeText {
       id: ltName
       text: warlock.name
       anchors.top: parent.top
       anchors.verticalCenter: parent.verticalCenter
       height: 0.1 * parent.height
       width: 0.3 * parent.width
       bg_color: "#210430"
       color: "#A8F4F4"
    }

    IconInfo {
        id: iiHP
        source: "qrc:/res/heart.png";
        text: warlock.hp
        height: 0.15 * parent.height
        width: height
        anchors.top: parent.top
        anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
    }

    IconInfo {
        id: iiBanked
        source: "qrc:/res/banked_spell.png";
        text: ""
        height: 0.15 * parent.height
        width: height
        anchors.top: parent.top
        anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        visible: warlock.banked_spell !== ""
    }

    Item {
        id: iMonsters
        anchors.top: parent.top
        anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
        width: 0.5 * parent.width
        height: 0.22 * parent.height
    }

    Item {
        id: iCharm
        anchors.top: parent.top
        anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        width: 0.5 * parent.width
        height: 0.22 * parent.height
    }

    ScrollView {
        id: svGestures
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0.01 * parent.height
        anchors.left: parent.left
        anchors.right: iiLeft.left
        height: 0.59 * parent.height

        ListView {
            id: lwGestures
            anchors.fill: parent
            orientation: ListView.Horizontal
            model: warlock.print_g
            delegate: Item {
                id: lwiGesture
                height: parent.height
                width: iiLeft.width

                IconInfoR {
                    id: iiHL
                    source: "qrc:/res/"+lwGestures[index].l+".png"
                    iconVisible: lwGestures[index].lv
                    textVisible: false
                    height: iiLeft.height
                    width: parent.width
                    anchors.bottom: iiHR.top
                    anchors.bottomMargin: 0.01 * rWarlock.height
                    anchors.right: parent.right
                }

                IconInfoR {
                    id: iiHR
                    source: "qrc:/res/"+lwGestures[index].r+".png"
                    iconVisible: lwGestures[index].rv
                    textVisible: false
                    height: iiLeft.height
                    width: parent.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0.01 * rWarlock.height
                    anchors.right: parent.right
                }
            }
        }
    }

    IconInfoR {
        id: iiLeft
        source: "qrc:/res/stars_light.png";
        text: ""
        height: 0.29 * parent.height
        width: 0.7 * height
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 0.01 * parent.height
        anchors.right: parent.right
    }

    IconInfoR {
        id: iiRight
        source: "qrc:/res/stars_light.png";
        text: ""
        height: 0.29 * parent.height
        width: 0.7 * height
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0.01 * parent.height
        anchors.right: parent.right
    }



    Component.onCompleted: finishWarlockCreation();


    function finishWarlockCreation() {
        console.log("Warlock.qml.finishWarlockCreation");
    }
}
