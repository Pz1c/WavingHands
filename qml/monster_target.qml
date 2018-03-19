import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: rTarget
    x: 0
    y: 0
    //width: 100
    //height: 62

    property string mt_id: ""
    property string mt_label: ""
    property string mt_model: ""
    property string mt_value: ""

    Text {
       id: tLabel
       text: mt_label
       anchors.left: parent.left
       anchors.verticalCenter: mt_target.verticalCenter
    }

    ComboBox {
        id: mt_target
        anchors.left: tLabel.right
        width: 100
        //anchors.right: parent.right
        model: mt_model.split(";")
        onCurrentTextChanged: mt_value = mt_target.currentText
    }

    Component.onCompleted: finishCreation();

    function finishCreation() {
        rTarget.width = tLabel.width + 3 + mt_target.width
        rTarget.height = mt_target.height
    }
}
