import QtQuick 2.9
import QtQuick.Dialogs 1.2

Item {
    id: rColorPanel
    property var arr_colors: ["#FFFFFF", "#FF0000", "#00FF00", "#0000FF", "#60C1C2",
                              "#FFFF00", "#FF7F50", "#800080", "#000000"]
    property var arr_buttons: []

    signal colorChanged
    signal paletteChanged

    ColorItem {
        id: rColor0
        anchors.top: parent.top
        anchors.left: parent.left
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[0]

        onClicked: setColor(0)
    }

    ColorItem {
        id: rColor1
        anchors.top: parent.top
        anchors.left: rColor0.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[1]

        onClicked: setColor(1)
    }

    ColorItem {
        id: rColor2
        anchors.top: parent.top
        anchors.left: rColor1.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[2]

        onClicked: setColor(2)
    }

    ColorItem {
        id: rColor3
        anchors.top: parent.top
        anchors.left: rColor2.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[3]

        onClicked: setColor(3)
    }

    ColorItem {
        id: rColor4
        anchors.top: parent.top
        anchors.left: rColor3.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[4]

        onClicked: setColor(4)
    }

    ColorItem {
        id: rColor5
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[5]

        onClicked: setColor(5)
    }

    ColorItem {
        id: rColor6
        anchors.bottom: parent.bottom
        anchors.left: rColor5.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[6]

        onClicked: setColor(6)
    }

    ColorItem {
        id: rColor7
        anchors.bottom: parent.bottom
        anchors.left: rColor6.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[7]

        onClicked: setColor(7)
    }

    ColorItem {
        id: rColor8
        anchors.bottom: parent.bottom
        anchors.left: rColor7.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        color: arr_colors[8]

        onClicked: setColor(8)
    }

    Image {
        id: rColor9
        anchors.bottom: parent.bottom
        anchors.left: rColor8.right
        height: 0.5 * parent.height
        width: 0.2 * parent.width
        source: "qrc:///res/gui/color_picker.png"

        MouseArea {
            id: maPicker
            anchors.fill: parent
            onClicked: {
                if (current_idx !== 8) {
                    // 8 - black color, need to removing dots
                    colorDialog.visible = true;
                }
            }
        }


    }

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"

        onAccepted: {
            var str_color = "" + colorDialog.color;
            console.log("You chose", colorDialog.color, str_color);
            //rColor9.color = colorDialog.color;

            arr_colors[current_idx] = str_color;
            arr_buttons[current_idx].color = str_color;
            rColorPanel.colorChanged();
            rColorPanel.paletteChanged();
        }

        onRejected: {
            console.log("Canceled");
        }
        //Component.onCompleted: visible = true
    }

    property int current_idx: 0

    function getColor() {
        return arr_colors[current_idx];
    }

    function setColor(idx) {
        arr_buttons[current_idx].active = false;
        current_idx = idx;
        arr_buttons[current_idx].active = true;
        //if (idx === 9) {
        //    colorDialog.visible = true;
        //} else {
        rColorPanel.colorChanged();
        //}
    }

    function setPalette(string) {
        var new_arr = string.split(",");
        if (new_arr.length !== arr_colors.length) {
            return;
        }
        arr_colors = new_arr;
    }

    function getPalette() {
        return arr_colors.join(",");
    }



    Component.onCompleted: {
        arr_buttons.push(rColor0);
        arr_buttons.push(rColor1);
        arr_buttons.push(rColor2);
        arr_buttons.push(rColor3);
        arr_buttons.push(rColor4);
        arr_buttons.push(rColor5);
        arr_buttons.push(rColor6);
        arr_buttons.push(rColor7);
        arr_buttons.push(rColor8);
        //arr_buttons.push(rColor9);
        setColor(0);
    }
}
