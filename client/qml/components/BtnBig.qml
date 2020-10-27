import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool transparent: false
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property string bg_color_active: "lightskyblue"
    property string bg_color_inactive: "lightgray"
    property string border_color_active: "royalblue"
    property string border_color_inactive: "gray"

    signal clicked

    color: transparent ? "transparent" : (active ? bg_color_active : bg_color_inactive)
    radius: 5
    border.color: (active ? border_color_active : border_color_inactive)

    LargeText {
        id: btnText
        anchors.centerIn: parent
        height: 0.8 * parent.height
        width: 0.8 * parent.width
        color: "yellow"
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
