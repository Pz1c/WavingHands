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
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            //height: 50 * mainWindow.ratioObject
            color: "#FEE2D6"
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 28 * mainWindow.ratioFont
            text: dict.getStringByCode("SearchWizardShortDesc")
            wrapMode: Text.WordWrap
        }

        Text {
            id: ltDesc
            anchors.top: ltShortDesc.bottom
            anchors.topMargin: 20 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 20 * mainWindow.ratioObject
            font.pixelSize: 28 * mainWindow.ratioFont
            color: "#10C9F5"
            text: ""
            horizontalAlignment: Text.AlignLeft
            //fontSizeMode: Text.VerticalFit
        }

        ScrollView {
            id: svMain
            anchors.top: ltDesc.bottom
            anchors.topMargin: 20 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 20 * mainWindow.ratioObject
            anchors.bottom: ltiLogin.top
            anchors.bottomMargin: 20 * mainWindow.ratioObject

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            contentWidth: -1

            Text {
                id: ltWarlockDetails
                width: dialogWindow.width * 0.99
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                font.pixelSize: 28 * mainWindow.ratioFont
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignJustify
            }
        }

        LabeledTextInput {
            id: ltiLogin
            anchors.bottom: dialogWindow.bottom
            anchors.bottomMargin: 20 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: iiCSend.left
            anchors.rightMargin: 5 * mainWindow.ratioObject
            height: 128 * mainWindow.ratioObject
            marginPrcLeft: 0
            //width: dialogWindow.width
            fontTitle.pixelSize: 28 * mainWindow.ratioFont
            fontEdit.pixelSize: 28 * mainWindow.ratioFont
            title: dict.getStringByCode("SearchWizardInputTitle")
            title_color: "#E7FFFF"
            border_color: "#E7FFFF"
            text_color: "#E7FFFF"
            transparent: true
            placeholderText: "e.g. Galbarad"
            placeholderTextColor: "#544653"
            regularExpression: /^[a-zA-Z0-9_-]{2,10}$/

            onInputChanged: {
                var str = mainWindow.gameCore.getWarlockStats(ltiLogin.text);
                console.log("onInputChanged", ltiLogin.text, str);
                var ws = JSON.parse(str);
                if (ws.found) {
                    iiCSend.active = true;
                    iiCSend.source = "qrc:/res/send_1.png"
                    ltDesc.text = "Start " + (battleType === "vf" ? "training " : "") + "battle with " + ws.name + "?";
                    ltWarlockDetails.text = "Score: " + ws.elo + "<br>Played: " + ws.played + "<br>Won: " + ws.won + "<br>Died: " + ws.died;
                } else {
                    iiCSend.active = false;
                    iiCSend.source = "qrc:/res/send_0.png"
                    ltDesc.text = "Not found warlock with name " + ws.name;
                    ltWarlockDetails.text = "";
                }
            }
        }

        IconInfo {
            id: iiCSend
            source: "qrc:/res/send_0.png"
            textVisible: false

            height: 64 * mainWindow.ratioObject
            width: 64 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 20 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 17 * mainWindow.ratioObject
            active: false
            color: "transparent"
            //text_color: "#E7FFFF"
            //text: "Submit"

            onClicked: {
                console.log("try to find", ltiLogin.text);
                // bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count, int FriendlyLevel, QString Description, QString Warlock = ""
                mainWindow.gameCore.createNewChallenge(true, true, true, true, 2, (battleType === "vf" ? 2 : 1), "Join to fun)", ltiLogin.text);
                mainWindow.processEscape();
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

    property string battleType: "vf"

    function initErrFields() {
        console.log("wnd_search_warlock.initErrFields", JSON.stringify(mainWindow.gERROR));
        if (mainWindow.gERROR.bfl) {
            battleType = mainWindow.gERROR.bfl;
        }

        //ltError.text = replaceAll(mainWindow.gERROR.text, "&quot;", '"');
        //title_text = mainWindow.gERROR.title;
        /*if (mainWindow.gERROR.msg && (mainWindow.gERROR.msg.length > 0)) {
            ltiMsg.text = mainWindow.gERROR.msg;
        } else {
            ltiMsg.text = "";
        }*/

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
