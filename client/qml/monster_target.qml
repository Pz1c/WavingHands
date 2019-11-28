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
    property int mt_is_player_owner: 0
    property int mt_is_just_created: 0
    property int mt_is_charm: 0

    Text {
       id: tLabel
       text: mt_label
       anchors.left: parent.left
       anchors.verticalCenter: cbTarget.verticalCenter
    }

    ComboBox {
        id: cbTarget
        anchors.left: tLabel.right
        anchors.right: parent.right
        //width: 100
        //anchors.right: parent.right
        model: mt_model.split(";")
        onCurrentTextChanged: mt_value = cbTarget.currentText
    }

    Component.onCompleted: finishCreation();

    function setTarget(mt_target) {
        console.log("monster_traget.setTarget", mt_target, cbTarget.model);
        var idx = -1;
        if (mt_target !== '') {
            for(var i = 0, Ln = cbTarget.model.length; i < Ln; ++i) {
                if (cbTarget.model[i] === mt_target) {
                    idx = i;
                    break;
                }
            }
        } else {
            idx = 0;
        }

        if (idx !== -1) {
            cbTarget.currentIndex = idx;
            mt_value = cbTarget.model[idx];
        }
    }

    function finishCreation() {
        //rTarget.width = tLabel.width + 3 + mt_target.width
        rTarget.height = cbTarget.height;
    }
}
