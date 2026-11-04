// https://github.com/Pz1c/WavingHands/issues/149, /issues/216, /issues/336, /issues/366
// Settings, from the main menu. Four entries, each a SettingsSwitch bound to a core property:
// "Auto start matches" (Keep me On in #149, the auto matching of #216) is keepMeOn
// (QWarloksDuelCore::setKeepMeOn); "Fast gesture selection" (#366) is fastGesture
// (QWarloksDuelCore::setFastGesture), read by wnd_gesture.qml; "Fast target selection" is
// fastTarget (QWarloksDuelCore::setFastTarget), read by wnd_target.qml; "Game notifications" (#336)
// is pushNotifications (QWarloksDuelCore::setPushNotifications), which registers this
// phone's push token with the caster server - after the system's notification permission
// dialog when that is still open.
// Under it, "Send notification token" registers the token again on demand
// (QWarloksDuelCore::sendPushTokenNow) and shows the server's answer (pushTokenStatus).

import QtQuick 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        // scrolls when the entries do not fit (small screens, longer DE/ES texts)
        Flickable {
            id: fItem
            anchors.fill: parent
            contentWidth: width
            contentHeight: ltPushTokenStatus.y + ltPushTokenStatus.height + 24 * mainWindow.ratioObject
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            z: 17

            Item {
                id: iItem
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 24 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 24 * mainWindow.ratioObject
                height: fItem.contentHeight

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
                    id: ssFastGesture
                    anchors.top: ltKeepMeOnDesc.bottom
                    anchors.topMargin: 48 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: dict.getStringByCode("FastGesture")
                    checked: mainWindow.gameCore.fastGesture
                    onToggled: function (on) { dMainItem.setFastGesture(on); }
                }

                Text {
                    id: ltFastGestureDesc
                    anchors.top: ssFastGesture.bottom
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: 24 * mainWindow.ratioFont
                    color: "#A8F4F4"
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    text: dict.getStringByCode("FastGestureSettingDesc")
                }

                SettingsSwitch {
                    id: ssFastTarget
                    anchors.top: ltFastGestureDesc.bottom
                    anchors.topMargin: 48 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: dict.getStringByCode("FastTarget")
                    checked: mainWindow.gameCore.fastTarget
                    onToggled: function (on) { dMainItem.setFastTarget(on); }
                }

                Text {
                    id: ltFastTargetDesc
                    anchors.top: ssFastTarget.bottom
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: 24 * mainWindow.ratioFont
                    color: "#A8F4F4"
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    text: dict.getStringByCode("FastTargetSettingDesc")
                }

                SettingsSwitch {
                    id: ssPushNotifications
                    anchors.top: ltFastTargetDesc.bottom
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

                // Registers this phone's token with the caster server again, whatever the app
                // believes about it, and shows the server's answer below.
                BtnBig {
                    id: bbSendPushToken
                    anchors.top: ltPushNotificationsDesc.bottom
                    anchors.topMargin: 24 * mainWindow.ratioObject
                    anchors.left: parent.left
                    width: 420 * mainWindow.ratioObject
                    height: 72 * mainWindow.ratioObject
                    radius: 18 * mainWindow.ratioObject
                    border.width: 0
                    text_color: "#ABF4F4"
                    font.pixelSize: 28 * mainWindow.ratioFont
                    text: dict.getStringByCode("SendPushToken")
                    gradient: Gradient {
                        GradientStop { position: 0.0 ; color: "#905B93" }
                        GradientStop { position: 0.75; color: "#551470" }
                        GradientStop { position: 1.0 ; color: "#551470" }
                    }
                    onClicked: dMainItem.sendPushToken()
                }

                Text {
                    id: ltPushTokenStatus
                    anchors.top: bbSendPushToken.bottom
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: 24 * mainWindow.ratioFont
                    color: "#A8F4F4"
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    text: mainWindow.gameCore.pushTokenStatus
                }
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

    // https://github.com/Pz1c/WavingHands/issues/366
    function setFastGesture(on) {
        console.log("wnd_settings.setFastGesture", on);
        mainWindow.logEvent("Settings_FastGesture", {On: on ? 1 : 0});
        mainWindow.gameCore.setFastGesture(on);
    }

    function setFastTarget(on) {
        console.log("wnd_settings.setFastTarget", on);
        mainWindow.logEvent("Settings_FastTarget", {On: on ? 1 : 0});
        mainWindow.gameCore.setFastTarget(on);
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

    function sendPushToken() {
        console.log("wnd_settings.sendPushToken");
        mainWindow.logEvent("Settings_SendPushToken", {});
        mainWindow.gameCore.sendPushTokenNow();
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
