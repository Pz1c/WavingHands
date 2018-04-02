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
    property var w_possible_spells: []
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
       fontSizeMode: Text.Fit
       //font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelLeft
       text: dict.getStringByCode("Left")
       anchors.verticalCenter: cbPossibleLeft.verticalCenter
       anchors.left: parent.left
       width: 0.2 * parent.width
       fontSizeMode: Text.Fit
       //font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelLeftGesture
       text: w_left_g
       anchors.right: cbPossibleLeft.left
       anchors.verticalCenter: cbPossibleLeft.verticalCenter
       fontSizeMode: Text.Fit
       textFormat: Text.PlainText
       width: 0.3 * parent.width
       //font.pointSize: 13 * height_koeff
    }

    ComboBox {
        id: cbPossibleLeft
        anchors.top: tLabel.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        width: 0.5 * parent.width
        model: []
        visible: false
        onCurrentTextChanged: setGesture(1)
    }

    Text {
       id: tLabelRight
       text: dict.getStringByCode("Right")
       anchors.verticalCenter: cbPossibleRight.verticalCenter
       anchors.left: parent.left
       fontSizeMode: Text.Fit
       width: tLabelLeft.width
       //font.pointSize: 13 * height_koeff
    }

    Text {
       id: tLabelRightGesture
       text: w_right_g
       anchors.right: cbPossibleRight.left
       anchors.verticalCenter: cbPossibleRight.verticalCenter
       //font.pointSize: 13 * height_koeff
       textFormat: Text.PlainText
       fontSizeMode: Text.Fit
       width: tLabelLeftGesture.width
    }

    ComboBox {
        id: cbPossibleRight
        anchors.top: cbPossibleLeft.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        model: []
        width: cbPossibleLeft.width
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
        //var idx3 = spell_text.indexOf(" ");
        var spell = spell_text.substr(idx1 - idx, 1);
        console.log("setGesture", left, spell_text, idx1, idx2, idx, spell);
        Qt.mainWindow.changeGesture(spell, left);
    }

    function finishCreation() {
        console.log("Warlock.qml finishCreation", w_possible_spells, w_left_g, w_right_g)
        //rWarlock.width = tLabelRight.width + 12 + tLabelRightGesture.width + cbPossibleRight.width
        var dead_or_surrender = (w_warlock_status.indexOf("Surrendered") != -1) || (w_warlock_status.indexOf("Health: 0") != -1);
        rWarlock.height = tLabel.height + 15 + cbPossibleLeft.height + cbPossibleRight.height;
        var is_possible = (w_possible_spells.length > 0) && !dead_or_surrender;
        if (is_possible) {
            var arr = w_possible_spells;//.split("#");
            var arr_l = [];
            var arr_r = [];
            for(var i = 0, Ln = arr.length; i < Ln; ++i) {
                if (arr[i].a === 0) {
                    continue;
                }

                var spell_text = arr[i].g + '(' + arr[i].t + ') ' + arr[i].n;
                if (arr[i].h === 1) { // left
                    arr_l.push(spell_text);
                } else if (arr[i].h === 2) { // right
                    arr_r.push(spell_text);
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
        }
        var search_login = Qt.core.login + "";
        w_send_gestures = w_warlock_status.toLocaleLowerCase().indexOf(search_login.toLowerCase()) !== -1;
        console.log("warlock.qml", search_login, w_warlock_status, w_send_gestures);
        if (w_send_gestures) {
            tLabel.color = "green";
            if (is_possible) {
                setGesture(1);
                setGesture(0);
            }
        } else if (dead_or_surrender) {
            tLabel.color = "grey";
        } else {
            tLabel.color = "red";
        }
    }
}
