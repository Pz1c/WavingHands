import QtQuick 2
//import "qrc:/js/text_utils.js" as TU
//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool transparent: false
    property alias image: btnIcon.source
    property alias image_anchors: btnIcon.anchors
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias fontPixelSize: btnText.font.pixelSize
    property alias mouseEnabled: maItem.enabled
    property alias text_width: btnText.width
    property alias icon_width: btnIcon.width
    property alias text_height: btnText.height
    property alias icon_height: btnIcon.height

    signal clicked

    color: transparent ? "transparent" : "black"
    z: 100

    LargeText {
        id: btnText
        anchors.centerIn: parent
        color: "white"
        text: "0"
        height: 0.9 * button.height
        width: 0.5 * button.width
    }

    Image {
        id: btnIcon
        fillMode: Image.Stretch
        source: "qrc:/res/back_arrow.png";
        height: 0.9 * button.height
        width: 0.9 * button.height
        anchors.verticalCenter: button.verticalCenter
    }

    //Mouse area to react on click events
    MouseArea {
        id: maItem
        anchors.fill: button
        onClicked: {
            button.clicked();
        }
    }
}
