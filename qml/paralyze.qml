import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: rParalyze
    x: 0
    y: 0

    property string p_label: ""
    property string p_target_id: ""
    property string p_hand_value: "LH"
    property var dict: Qt.mainWindow.warlockDictionary

    Text {
       id: tLabel
       text: p_label
       anchors.left: parent.left
       anchors.verticalCenter: p_hand.verticalCenter
    }

    ComboBox {
        id: p_hand
        //editable: false
        anchors.left: tLabel.right
        //anchors.right: parent.right
        model: [dict.getStringByCode("Left"), dict.getStringByCode("Right")]
        width: 100//parent.width
        onCurrentIndexChanged: p_hand_value = p_hand.currentIndex == 0 ? "LH" : "RH"
    }

    Component.onCompleted: finishCreation();

    function finishCreation() {
        //rParalyze.width = tLabel.width + 3 + p_hand.width
        rParalyze.height = p_hand.height
    }
}
