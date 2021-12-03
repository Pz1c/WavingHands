// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    //with_controls: true
    //with_apply: true
    //body_height_prc: 60

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        //color: "transparent"

        Text {
            id: ltTitle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            //height: 70 * mainWindow.ratioObject
            font.pixelSize: 49 * mainWindow.ratioFont
            color: "#10C9F5"
            horizontalAlignment: Text.AlignLeft
            //fontSizeMode: Text.VerticalFit
            text: dict.getStringByCode("SearchWizardTitle")
        }

        Text {
            id: ltShortDesc
            anchors.top: ltTitle.bottom
            anchors.topMargin: 20 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.right: parent.right
            //height: 50 * mainWindow.ratioObject
            color: "#FEE2D6"
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 28 * mainWindow.ratioFont
            text: dict.getStringByCode("SearchWizardShortDesc")
            wrapMode: Text.WordWrap
        }

        LabeledTextInput {
            id: ltiLogin
            anchors.bottom: dialogWindow.bottom
            anchors.bottomMargin: 2 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: iiCSend.left
            anchors.rightMargin: 20 * mainWindow.ratioObject
            height: 128 * mainWindow.ratioObject
            //width: dialogWindow.width
            title: dict.getStringByCode("SearchWizardInputTitle")
            title_color: "lightgray"
            //text_color: "black"
            transparent: false
            placeholderText: "Warlock"
            regularExpression: /^[a-zA-Z0-9_-]{2,10}$/
        }

        IconInfo {
            id: iiCSend
            source: "qrc:/res/send_1.png"
            textVisible: false

            height: 64 * mainWindow.ratioObject
            width: 64 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 20 * mainWindow.ratioObject
            anchors.verticalCenter: ltiLogin.verticalCenter
            active: true
            color: "transparent"
            //text_color: "#E7FFFF"
            //text: "Submit"

            onClicked: {
                console.log("try to find", ltiLogin.text);
            }
        }

        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    /*onApply: {
        mainWindow.storeBattleChatMsg(ltiMsg.text);
    }*/

    function showWnd() {
        initErrFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function initErrFields() {
        console.log("wnd_chat.initErrFields", JSON.stringify(mainWindow.gERROR));
        ltError.text = replaceAll(mainWindow.gERROR.text, "&quot;", '"');
        title_text = mainWindow.gERROR.title;
        if (mainWindow.gERROR.msg && (mainWindow.gERROR.msg.length > 0)) {
            ltiMsg.text = mainWindow.gERROR.msg;
        } else {
            ltiMsg.text = "";
        }

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
        ltiMsg.setFontSize(0.20 * dialogWindow.height);
    }
}
