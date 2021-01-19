import QtQuick 2.9

Rectangle {
    id: rColor

    signal clicked
    property bool active: false

    border.color: "lightgrey"
    border.width: 2

    MouseArea {
        id: maColor
        anchors.fill: parent

        onClicked: {
            rColor.clicked();
        }
    }

    onActiveChanged: {
        if (active) {
            rColor.border.color = "green";
        } else {
            rColor.border.color = "lightgrey";
        }
    }
}
