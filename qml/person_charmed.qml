import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: rPersonCharmed
    x: 0
    y: 0

    property string pc_label: ""
    property string pc_target_id: ""
    property string pc_hand_value: "LH"
    property string pc_gesture_value: "-"
    property var dict: Qt.mainWindow.warlockDictionary

    Text {
       id: tLabel
       text: pc_label
       anchors.left: parent.left
       anchors.verticalCenter: pc_gesture.verticalCenter
    }

    ComboBox {
        id: pc_hand
        anchors.left: tLabel.right
        anchors.leftMargin: 3
        anchors.verticalCenter: pc_gesture.verticalCenter
        model: [dict.getStringByCode("Left"), dict.getStringByCode("Right")]
        width: 95
        onCurrentIndexChanged: pc_hand_value = pc_hand.currentIndex == 0 ? "LH" : "RH"
    }

    ComboBox {
        id: pc_gesture
        anchors.left: pc_hand.right
        anchors.leftMargin: 3
        //editable: false
        model: ["-", ">", "C", "D", "F", "P", "S", "W"]
        width: 95
        onCurrentTextChanged: pc_gesture_value = pc_gesture.currentText
    }

    Component.onCompleted: finishCreation();

    function finishCreation() {
        rPersonCharmed.width = tLabel.width + 4 + pc_hand.width + pc_gesture.width
        rPersonCharmed.height = pc_hand.height
    }
}
