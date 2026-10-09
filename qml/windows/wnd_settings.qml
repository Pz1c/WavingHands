// https://github.com/Pz1c/WavingHands/issues/149, /issues/216
// Settings, from the main menu. One entry so far: the "Auto start matches" switch (Keep me On
// in #149), which is the core's keepMeOn property (QWarloksDuelCore::setKeepMeOn).

import QtQuick 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Item {
            id: iItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            z: 17

            Text {
                id: ltTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignLeft
                text: dict.getStringByCode("Settings")
            }

            Item {
                id: iKeepMeOn
                anchors.top: ltTitle.bottom
                anchors.topMargin: 48 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                height: 72 * mainWindow.ratioObject

                Text {
                    id: ltKeepMeOn
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: rSwitch.left
                    anchors.rightMargin: 24 * mainWindow.ratioObject
                    font.pixelSize: 34 * mainWindow.ratioFont
                    color: "#FEE2D6"
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    text: dict.getStringByCode("AutoMatch")
                }

                // ON / OFF switch; follows the core, the click asks the core to change it
                Rectangle {
                    id: rSwitch
                    property bool checked: mainWindow.gameCore.keepMeOn
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    width: 132 * mainWindow.ratioObject
                    height: 60 * mainWindow.ratioObject
                    radius: height / 2
                    color: checked ? "#2DA0A5" : "#544653"
                    border.color: "#A8F4F4"
                    border.width: 2

                    Rectangle {
                        id: rKnob
                        width: rSwitch.height - 8 * mainWindow.ratioObject
                        height: width
                        radius: width / 2
                        anchors.verticalCenter: parent.verticalCenter
                        x: rSwitch.checked ? rSwitch.width - width - 4 * mainWindow.ratioObject : 4 * mainWindow.ratioObject
                        color: "#E7FFFF"

                        Behavior on x { NumberAnimation { duration: 150 } }
                    }

                    Text {
                        id: tSwitch
                        anchors.verticalCenter: parent.verticalCenter
                        x: rSwitch.checked ? 14 * mainWindow.ratioObject : rSwitch.width - width - 14 * mainWindow.ratioObject
                        font.pixelSize: 21 * mainWindow.ratioFont
                        color: "#E7FFFF"
                        text: dict.getStringByCode(rSwitch.checked ? "SwitchOn" : "SwitchOff")
                    }

                    MouseArea {
                        id: maSwitch
                        anchors.fill: parent
                        onClicked: {
                            dMainItem.setKeepMeOn(!rSwitch.checked);
                        }
                    }
                }
            }

            Text {
                id: ltKeepMeOnDesc
                anchors.top: iKeepMeOn.bottom
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 24 * mainWindow.ratioFont
                color: "#A8F4F4"
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("AutoMatchSettingDesc")
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function setKeepMeOn(on) {
        console.log("wnd_settings.setKeepMeOn", on);
        mainWindow.logEvent("Settings_KeepMeOn", {On: on ? 1 : 0});
        mainWindow.gameCore.setKeepMeOn(on);
    }

    function showWnd() {
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
    }
}
