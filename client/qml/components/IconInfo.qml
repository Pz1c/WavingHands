import QtQuick 2.9

//Implementation of the Button control.
Image {
    id: button

    property bool active: true
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode

    signal clicked

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
