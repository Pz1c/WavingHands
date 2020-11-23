// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: false
    body_height_prc: 60

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: svError
            anchors.fill: parent

                Text {
                    id: ltError
                    y: 0
                    x: 0
                    width: dialogWindow.width - 0.11 * dialogWindow.height
                    //height: svLog.height
                    font.pixelSize: 0.05 * dialogWindow.height
                    wrapMode: Text.WordWrap
                    color: "lightgrey"
                    textFormat: Text.RichText

                    onLinkActivated: {
                        mainWindow.linkActivated(link);
                        mainWindow.processEscape();
                    }
                }
        }

        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    onAction1: {
        mainWindow.openBattleOnline();
        mainWindow.processEscape();
    }

    function showWnd() {
        initFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initErrFields() {
        console.log("wnd_error.initErrFields", JSON.stringify(mainWindow.gERROR));
        if (!mainWindow.gERROR || !mainWindow.gERROR.id) {
            return;
        }
        ltError.text = mainWindow.gERROR.text;
        switch(mainWindow.gERROR.type) {
        case 1:
            title_text = dict.getStringByCode("WndInfoTitle");
            title_color = "black";
            break;
        case 2: // battle info
            with_action1 = true;
            action1_text = dict.getStringByCode("WatchOnline");
            break;
        default:
            title_text = dict.getStringByCode("WndErrorTitle");
            title_color = "red";
            break;
        }
        if (mainWindow.gERROR.title) {
            title_text = mainWindow.gERROR.title;
        }

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        initErrFields();
    }
}
