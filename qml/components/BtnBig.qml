import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool transparent: false
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias font: btnText.font
    property string bg_color_active: "lightskyblue"
    property string bg_color_inactive: "lightgray"
    property string border_color_active: "royalblue"
    property string border_color_inactive: "gray"
    property alias textObj: btnText
    // optional second line under the text, empty keeps the text centred alone
    property alias subtitle: btnSubtitle.text
    property alias subtitleObj: btnSubtitle

    signal clicked

    color: transparent ? "transparent" : (active ? bg_color_active : bg_color_inactive)
    radius: transparent ? 0 : 5
    border.color: (active ? border_color_active : border_color_inactive)

    LargeText {
        id: btnText
        anchors.centerIn: parent
        anchors.verticalCenterOffset: btnSubtitle.visible ? -0.15 * parent.height : 0
        height: (btnSubtitle.visible ? 0.5 : 0.8) * parent.height
        width: 0.8 * parent.width
        color: "yellow"
        fontSizeMode: Text.Fit
    }

    Text {
        id: btnSubtitle
        anchors.top: btnText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 0.3 * parent.height
        width: 0.8 * parent.width
        visible: text !== ""
        color: btnText.color
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: ""
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
