import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"

Item {
    id: rWarlock

    property var l_warlock: ({})


    LargeText {
       id: ltName
       text: l_warlock.name
       anchors.top: parent.top
       anchors.horizontalCenter: parent.horizontalCenter
       height: 0.1 * parent.height
       width: 0.3 * parent.width
       bg_color: l_warlock.player ? "#210430" : "#544653"
       color: l_warlock.player ? "#A8F4F4" : "#FEE2D6"
    }

    IconInfo {
        id: iiHP
        source: "qrc:/res/heart.png";
        text: l_warlock.hp
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
        visible: l_warlock.banked_spell !== ""
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
        anchors.left: parent.left
        anchors.right: iiLeft.left
        anchors.bottom: rBottomLine.top
        anchors.top: iCharm.bottom

        ListView {
            id: lwGestures
            anchors.fill: parent
            orientation: ListView.Horizontal
            model: l_warlock.print_g
            delegate: Item {
                id: lwiGesture
                height: iiLeft.height
                width: iiLeft.width

                IconInfoR {
                    id: iiHL
                    source: "qrc:/res/"+lwGestures.model[index].l+".png"
                    iconVisible: lwGestures.model[index].lv
                    textVisible: false
                    height: iiLeft.height
                    width: parent.width
                    anchors.bottom: iiHR.top
                    anchors.bottomMargin: 0.01 * rWarlock.height
                    anchors.right: parent.right
                }

                IconInfoR {
                    id: iiHR
                    source: "qrc:/res/"+lwGestures.model[index].r+".png"
                    iconVisible: lwGestures.model[index].rv
                    textVisible: false
                    height: iiLeft.height
                    width: parent.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0.05 * rWarlock.height
                    anchors.right: parent.right
                }
            }
        }
    }

    IconInfoR {
        id: iiLeft
        source: "qrc:/res/stars_light.png";
        text: ""
        height: 0.37 * parent.height
        width: 0.7 * height
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 0.01 * parent.height
        anchors.right: parent.right
    }

    IconInfoR {
        id: iiRight
        source: "qrc:/res/stars_light.png";
        text: ""
        height: iiLeft.height
        width: iiLeft.width
        anchors.bottom: rBottomLine.top
        anchors.right: parent.right
    }

    Rectangle {
        id: rBottomLine
        height: 0.03 * parent.height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        color: "#FEE2D6"
    }



    Component.onCompleted: finishWarlockCreation();


    function finishWarlockCreation() {
        console.log("Warlock.qml.finishWarlockCreation");
    }
}
