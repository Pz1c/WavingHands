import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: rPersonCharmed
    x: 0
    y: 0
    width: parent.width

    property string pc_label: ""
    property string pc_target_id: ""
    property string pc_target_name: ""
    property string pc_hand_value: "LH"
    property string pc_gesture_value: "-"
    property var dict: Qt.mainWindow.warlockDictionary

    Text {
       id: tLabel
       text: pc_label
       anchors.left: parent.left
       anchors.top: parent.top
       height: parent.height
       width: 0.6 * parent.width
    }

    ComboBox {
        id: pc_hand
        anchors.left: tLabel.right
        anchors.right: pc_gesture.left
        //anchors.leftMargin: 0.01 * parent.width
        anchors.verticalCenter: pc_gesture.verticalCenter
        model: [dict.getStringByCode("Left"), dict.getStringByCode("Right")]
        width: 0.22 * parent.width
        onCurrentIndexChanged: pc_hand_value = pc_hand.currentIndex == 0 ? "LH" : "RH"
    }

    ComboBox {
        id: pc_gesture
        anchors.right: parent.right
        model: ["-", ">", "C", "D", "F", "P", "S", "W"]
        width: 0.18 * parent.width
        height: parent.height
        onCurrentTextChanged: pc_gesture_value = pc_gesture.currentText
    }

    function setHandGesture(hand, gesture) {
        pc_hand.currentIndex = hand === "LH" ? 0 : 1;
        for(var i = 0, Ln = pc_gesture.model.length; i < Ln; ++i) {
            if (gesture === pc_gesture.model[i]) {
                pc_gesture.currentIndex = i;
                break;
            }
        }
    }

    Component.onCompleted: finishCreation();

    function finishCreation() {
        rPersonCharmed.height = pc_hand.height
    }
}
