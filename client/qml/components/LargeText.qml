import QtQuick 2.9
//import QtQuick.Controls 2.2

Item {
    id: ltRoot
    property alias bg_visible: ltBG.visible
    property alias bg_color: ltBG.color
    property bool  border_visible: true
    property alias color: tTitle.color
    property alias horizontalAlignment: tTitle.horizontalAlignment
    property alias verticalAlignment: tTitle.verticalAlignment
    property alias fontSizeMode: tTitle.fontSizeMode
    property alias text: tTitle.text
    property alias wrapMode: tTitle.wrapMode
    property alias font: tTitle.font

    Rectangle {
        id: ltBG
        anchors.fill: parent
        z: 1

        border.color: tTitle.color
        border.width: border_visible ? 1 : 0
        color: "black"
        visible: false
    }

    Text {
        id: tTitle
        anchors.fill: parent
        z: 2

        //fontSizeMode: Text.Fit
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 72
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        //font.family: Qt.defaultFontFamily
    }
}

//Implementation of the Button control.
/*Label {
    property alias bg_visible: rTitleBg.visible
    property alias bg_color: rTitleBg.color
    //property alias bg_border_color: rTitleBg.border.color
    //property alias bg_border_width: rTitleBg.border.width

    id: tTitle
    //fontSizeMode: Text.Fit
    fontSizeMode: Text.VerticalFit
    font.pixelSize: 72
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.family: Qt.defaultFontFamily

    background: Rectangle {
        //property alias border_color: border.color
        //property alias border_width: border.width

        id: rTitleBg
        color: "white"
        visible: false

//        border {
//            id: bg_border
//            color: "red"
//            width: 1
//        }

        border.color: tTitle.color
        border.width: 1
    }
}
*/
