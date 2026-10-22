// https://github.com/Pz1c/WavingHands/issues/149, /issues/336
// One row of the Settings window: a label and the hand-made ON / OFF switch. The switch
// follows `checked` (bind it to the core's property); a click only emits toggled(on), the
// window asks the core to change the setting.

import QtQuick 2.15
import ua.sp.warloksduel 2.0

Item {
    id: iRoot

    property alias text: ltLabel.text
    property bool checked: false
    // the window's scale factors, read once here
    readonly property real ratioObject: mainWindow.ratioObject
    readonly property real ratioFont: mainWindow.ratioFont

    signal toggled(bool on)

    height: 72 * iRoot.ratioObject

    Text {
        id: ltLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: rSwitch.left
        anchors.rightMargin: 24 * iRoot.ratioObject
        font.pixelSize: 34 * iRoot.ratioFont
        color: "#FEE2D6"
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }

    Rectangle {
        id: rSwitch
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: 132 * iRoot.ratioObject
        height: 60 * iRoot.ratioObject
        radius: height / 2
        color: iRoot.checked ? "#2DA0A5" : "#544653"
        border.color: "#A8F4F4"
        border.width: 2

        Rectangle {
            id: rKnob
            width: rSwitch.height - 8 * iRoot.ratioObject
            height: width
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            x: iRoot.checked ? rSwitch.width - width - 4 * iRoot.ratioObject : 4 * iRoot.ratioObject
            color: "#E7FFFF"

            Behavior on x { NumberAnimation { duration: 150 } }
        }

        Text {
            id: tSwitch
            anchors.verticalCenter: parent.verticalCenter
            x: iRoot.checked ? 14 * iRoot.ratioObject : rSwitch.width - width - 14 * iRoot.ratioObject
            font.pixelSize: 21 * iRoot.ratioFont
            color: "#E7FFFF"
            text: WarlockDictionary.getStringByCode(iRoot.checked ? "SwitchOn" : "SwitchOff")
        }

        MouseArea {
            id: maSwitch
            anchors.fill: parent
            onClicked: {
                iRoot.toggled(!iRoot.checked);
            }
        }
    }
}
