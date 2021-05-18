import QtQuick 2

Rectangle {
    id: rCB

    signal clicked
    property bool checked: false
    property alias innerBorder: rInner.border
    property alias innerRadius: rInner.radius
    property string fill_color_checked: "black"
    property string fill_color_unchecked: "white"

    border.color: "lightgrey"
    border.width: 3
    radius: 5

    Rectangle {
        id: rInner
        border.color: "black"
        border.width: 2
        radius: 5
        anchors.centerIn: parent
        width: parent.width - radius - parent.radius
        height: parent.height - radius - parent.radius
        color: checked ? fill_color_checked : fill_color_unchecked
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
