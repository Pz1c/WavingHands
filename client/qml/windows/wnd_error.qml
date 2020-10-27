// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: false

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.centerIn: content_item
        height: 0.8 * parent.height
        width: 0.95 * parent.width
        //focus: true

        color: "black"
        opacity: 0.8

        ScrollView {
            id: svError
            anchors.top: parent.top
            //anchors.topMargin: 0.05 * parent.height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.05 * parent.height
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.leftMargin: 0.05 * parent.height
            anchors.right: parent.right
            anchors.rightMargin: 0.05 * parent.height

            Text {
                id: ltError
                y: 0
                x: 0
                width: dialogWindow.width - 0.11 * dialogWindow.height
                //height: svLog.height
                font.pixelSize: 0.6 * tTitle.font.pixelSize
                wrapMode: Text.WordWrap
                color: "white"

                onLinkActivated: {
                    Qt.mainWindow.linkActivated(link);
                    clickCancel();
                }
            }
        }
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

    function initFields() {
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
        initFields();
        Qt.gameField.storeWnd(dMainItem);
    }
}
