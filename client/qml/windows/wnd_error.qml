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
    Rectangle {
        id: dialogWindow
        anchors.fill: content_item
        color: "snow"

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
                    color: "black"

                    onLinkActivated: {
                        Qt.mainWindow.linkActivated(link);
                        Qt.gameField.processEscape();
                    }
                }
        }

        z: 11
    }

    onCancel: {
        Qt.gameField.processEscape();
    }

    function showWnd() {
        initFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initErrFields() {
        console.log("wnd_error.initErrFields", JSON.stringify(Qt.error));
        if (!Qt.error || !Qt.error.id) {
            return;
        }
        ltError.text = Qt.error.text;
        switch(Qt.error.type) {
        case 1:
            title_text = dict.getStringByCode("WndInfoTitle");
            title_color = "white";
            break;
        default:
            title_text = dict.getStringByCode("WndErrorTitle");
            title_color = "red";
            break;
        }
        Qt.error = {};
    }

    Component.onCompleted: {
        initErrFields();
    }
}
