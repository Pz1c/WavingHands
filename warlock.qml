import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: rWarlock
    x: 0
    y: 0
    width:parent.width

    property string w_warlock_status: ""
    property string w_left_g: ""
    property string w_right_g: ""
    property string w_possible_spells: ""
    property real height_koeff: Qt.height_coeff
    property var dict: Qt.mainWindow.warlockDictionary
    property bool w_send_gestures: false

    Text {
       id: tLabel
       text: w_warlock_status
       anchors.top: parent.top
       anchors.topMargin: 5
       anchors.left: parent.left
       anchors.right: parent.right
       font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelLeft
       text: dict.getStringByCode("Left")
       anchors.verticalCenter: cbPossibleLeft.verticalCenter
       anchors.left: parent.left
       font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelRight
       text: dict.getStringByCode("Right")
       anchors.verticalCenter: cbPossibleRight.verticalCenter
       anchors.left: parent.left
       font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelLeftGesture
       text: "<tt>" + w_left_g + "</tt>"
       anchors.right: cbPossibleLeft.left
       anchors.verticalCenter: cbPossibleLeft.verticalCenter
       font.pointSize: 13 * height_koeff
       textFormat: Text.RichText
    }

    Text {
       id: tLabelRightGesture
       text: "<tt>" + w_right_g + "</tt>"
       anchors.right: cbPossibleRight.left
       anchors.verticalCenter: cbPossibleRight.verticalCenter
       font.pointSize: 13 * height_koeff
       textFormat: Text.RichText
    }

    ComboBox {
        id: cbPossibleLeft
        anchors.top: tLabel.bottom
        anchors.topMargin: 5
        //anchors.left: tLabelLeft.right
        //anchors.leftMargin: 5
        anchors.right: parent.right
        //x: tLabelLeftGesture.x + tLabelLeftGesture.width + 3
        //y: tLabelLeftGesture.y
        //editable: false
        model: []
        width: 0.5 * parent.width
        visible: false
        onCurrentTextChanged: setGesture(1)
    }

    ComboBox {
        id: cbPossibleRight
        anchors.top: cbPossibleLeft.bottom
        anchors.topMargin: 5
        //anchors.left: tLabelRight.right
        //anchors.leftMargin: 5
        anchors.right: parent.right
        //x: tLabelRightGesture.x + tLabelRightGesture.width + 3
        //y: tLabelRightGesture.y
        //editable: false
        model: []
        width: 0.5 * parent.width
        visible: false
        onCurrentTextChanged: setGesture(0)
    }

    Component.onCompleted: finishCreation();

    function setGesture(left) {
        if (!w_send_gestures) {
            return;
        }
        var spell_text = left ? cbPossibleLeft.currentText : cbPossibleRight.currentText;
        //var gesture_text = left ? w_left_g : w_right_g;
        var idx1 = spell_text.indexOf("(");
        var idx2 = spell_text.indexOf(")");
        var idx = spell_text.substr(idx1 + 1, idx2 - idx1 - 1) * 1;
        var idx3 = spell_text.indexOf(" ");
        var spell = spell_text.substr(idx3 - idx, 1);
        console.log("setGesture", left, spell_text, idx1, idx2, idx, spell);
        Qt.mainWindow.changeGesture(spell, left);

    }

    function finishCreation() {
        console.log("Warlock.qml finishCreation", w_possible_spells)
        //rWarlock.width = tLabelRight.width + 12 + tLabelRightGesture.width + cbPossibleRight.width
        rWarlock.height = tLabel.height + 15 + cbPossibleLeft.height + cbPossibleRight.height;
        if (w_possible_spells.length == 0) {
            return;
        }
        var arr = w_possible_spells.split("#");
        var arr_l = [];
        var arr_r = [];
        for(var i = 0, Ln = arr.length; i < Ln; ++i) {
            if (!arr[i] || arr[i] === '' || arr[i] === ' ') {
                continue;
            }
            var arr2 = arr[i].split(";");
            if (arr2[0] === "L") {
                arr_l.push(arr2[1]);
            } else if (arr2[0] === "R") {
                arr_r.push(arr2[1]);
            }
        }
        if (arr_l.length != 0) {
            cbPossibleLeft.model = arr_l
            cbPossibleLeft.visible = true
        }
        if (arr_r.length != 0) {
            cbPossibleRight.model = arr_r
            cbPossibleRight.visible = true
        }
        var search_login = Qt.core.login + "";
        w_send_gestures = w_warlock_status.toLocaleLowerCase().indexOf(search_login.toLowerCase()) !== -1;
        if (w_send_gestures) {
            tLabel.text = '* ' + w_warlock_status;
            setGesture(1);
            setGesture(0);
        }
    }
}
