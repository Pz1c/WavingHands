import QtQuick 2

Rectangle {
    id: rCB

    signal clicked
    property bool checked: false
    //property alias innerBorder: rInner.border
    //property alias innerRadius: rInner.radius
    //property string fill_color_checked: "#E7FFFF"
    //property string fill_color_unchecked: "black"

    border.color: "#E7FFFF"
    border.width: 3
    radius: 5
    color: "black"

    LargeText {
        id: rInner
        color: rCB.border.color
        anchors.fill: parent
        text: checked ? "V" : ""
        bg_visible: true
        bg_color: rCB.color
    }

    MouseArea {
        id: maColor
        anchors.fill: parent

        onClicked: {
            checked = !checked;
            rCB.clicked();
        }
    }


}
