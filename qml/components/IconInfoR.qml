import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias source: iIcon.source
    property alias iconVisible: iIcon.visible
    property alias iconHeight: iIcon.height
    property alias iconWidth: iIcon.width
    property alias textVisible: btnText.visible

    signal clicked
    signal doubleClicked

    color: "transparent"
    border.color: "#E7FFFF"
    border.width: 3
    radius: 10

    Image {
        id: iIcon
        anchors.centerIn: parent
        height: parent.height
        width: parent.width
    }

    LargeText {
        id: btnText
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: 0.1 * parent.height
        width: 0.1 * parent.width
        color: "white"
        fontSizeMode: Text.Fit
    }

    //Mouse area to react on click events
    MouseArea {
        anchors.fill: button
        onClicked: {
            if (active) {
                button.clicked();
            }
        }

        onPressAndHold: {
            if (active) {
                button.doubleClicked();
            }
        }
    }

    function onFinishCreation() {
        console.log("Button.onFinishCreation");
    }

    Component.onCompleted: onFinishCreation();
}
