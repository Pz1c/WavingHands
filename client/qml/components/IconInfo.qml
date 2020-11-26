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
    property alias textVisible: btnText.visible

    property var l_data: ({})

    signal clicked

    color: "#551470"
    radius: 10

    Image {
        id: iIcon
        anchors.centerIn: parent;
        height: 0.9 * parent.height
        width: 0.9 * parent.height
    }

    LargeText {
        id: btnText
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 0.3 * parent.height
        width: 0.3 * parent.width
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
    }

    function onFinishCreation() {
        console.log("Button.onFinishCreation");
    }

    Component.onCompleted: onFinishCreation();
}
