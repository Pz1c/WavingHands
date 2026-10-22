// https://github.com/Pz1c/WavingHands/issues/149, /issues/216, /issues/336
// Settings, from the main menu. Two entries, each a SettingsSwitch bound to a core property:
// "Auto start matches" (Keep me On in #149, the auto matching of #216) is keepMeOn
// (QWarloksDuelCore::setKeepMeOn); "Game notifications" (#336) is pushNotifications
// (QWarloksDuelCore::setPushNotifications), which registers this phone's push token with the
// caster server - after the system's notification permission dialog when that is still open.

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

            // follows the core, the click asks the core to change it
            SettingsSwitch {
                id: ssKeepMeOn
                anchors.top: ltTitle.bottom
                anchors.topMargin: 48 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                text: dict.getStringByCode("AutoMatch")
                checked: mainWindow.gameCore.keepMeOn
                onToggled: function (on) { dMainItem.setKeepMeOn(on); }
            }

            Text {
                id: ltKeepMeOnDesc
                anchors.top: ssKeepMeOn.bottom
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 24 * mainWindow.ratioFont
                color: "#A8F4F4"
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("AutoMatchSettingDesc")
            }

            SettingsSwitch {
                id: ssPushNotifications
                anchors.top: ltKeepMeOnDesc.bottom
                anchors.topMargin: 48 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                text: dict.getStringByCode("PushNotifications")
                checked: mainWindow.gameCore.pushNotifications
                onToggled: function (on) { dMainItem.setPushNotifications(on); }
            }

            Text {
                id: ltPushNotificationsDesc
                anchors.top: ssPushNotifications.bottom
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 24 * mainWindow.ratioFont
                color: "#A8F4F4"
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("PushNotificationsSettingDesc")
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

    // https://github.com/Pz1c/WavingHands/issues/336
    function setPushNotifications(on) {
        console.log("wnd_settings.setPushNotifications", on);
        mainWindow.logEvent("Settings_PushNotifications", {On: on ? 1 : 0});
        if (on && !mainWindow.gameCore.isNotificationGranted()) {
            // The system dialog; the registration follows on resume, once it is granted.
            mainWindow.gameCore.askPermissions();
        }
        mainWindow.gameCore.setPushNotifications(on);
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
