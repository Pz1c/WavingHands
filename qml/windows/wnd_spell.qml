// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/battle_gui_utils.js" as BGU
import "qrc:/js/spell_gui_utils.js" as SGU

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
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignLeft
                //fontSizeMode: Text.VerticalFit
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
                //fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
            }

            Text {
                id: ltDesc
                anchors.top: ltShortDesc.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 28 * mainWindow.ratioFont
                color: "#10C9F5"
                text: "Details"
                horizontalAlignment: Text.AlignLeft
                //fontSizeMode: Text.VerticalFit
                visible: false
            }

            Rectangle {
                id: rDesc
                visible: true//!ltDesc.visible
                anchors.verticalCenter: ltDesc.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#10C9F5"
                width: 0.7 * parent.width
                height: 3 * mainWindow.ratioObject
            }

            ScrollView {
                id: svMain
                anchors.top: ltDesc.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                contentWidth: -1

                Text {
                    id: ltError
                    width: iItem.width * 0.99
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText
                    font.pixelSize: 28 * mainWindow.ratioFont
                    color: "#FEE2D6"
                    horizontalAlignment: Text.AlignJustify

                    onLinkActivated: function (link) { errorLinkActivated(link) }
                }
            }

            Gradient {
                id: gDef
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: dict.getStringByCode("MonsterSetTarget")
                border.width: 0
                radius: 30
                visible: false
                z: 30
                font.pixelSize: 49 * mainWindow.ratioFont
                gradient: gDef

                width: 366 * ratioObject
                height: 96 * ratioObject
                anchors.bottom: bbSkipAction.top
                anchors.bottomMargin: 50 * ratioObject
                anchors.horizontalCenter: parent.horizontalCenter


                onClicked: {
                    console.log("chooseMonsterTarget", JSON.stringify(l_data));
                    mainWindow.processEscape();
                    if (l_data.action === "game_with_player") {
                        mainWindow.startGameWithPlayerEx();
                    } else if (l_data.action === "game_with_bot") {
                        mainWindow.startGameWithBotEx();
                    } else if (l_data.action === "private_challenge") {
                        mainWindow.gameCore.acceptChallenge(l_data.id, true);
                    } else if (l_data.action === "rate_us") {
                        Qt.openUrlExternally("https://play.google.com/store/apps/details?id=net.is.games.WarlocksDuel");
                    } else if (l_data.action === "feedback") {
                        Qt.openUrlExternally("https://forms.gle/vW3tBW8595KtREZe8");
                    } else if (l_data.action === "joinus") {
                        Qt.openUrlExternally("https://www.facebook.com/WarlocksDuel");
                    } else if (l_data.action === "leave") {
                        // do nothing
                    } else if (l_data.action === "reg_info") {
                        Qt.openUrlExternally("https://games.ravenblack.net/rules/0/register.html");
                    } else if (l_data.action === "force") {
                        mainWindow.gameCore.forceSurrender(l_data.id, l_data.fst);
                    } else if (l_data.under_control) {
                        mainWindow.chooseMonsterTarget(ltTitle.text);
                    }
                }
            }

            BtnBig {
                id: bbSkipAction
                text_color: "#A8F4F4"
                text: "Skip"
                transparent: true
                font.underline: true
                border.width: 0
                radius: 30
                visible: false
                gradient: gDef

                width: 366 * ratioObject
                height: 60 * ratioObject
                font.pixelSize: 28 * mainWindow.ratioFont
                fontSizeMode: Text.VerticalFit

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 150 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    console.log("skip feedback action");
                    mainWindow.processEscape();
                    if (l_data.action === "leave") {
                        mainWindow.gameCore.leaveBattle(l_data.id);
                    } else if (l_data.action === "game_with_player") {
                        mainWindow.showShareWnd("f");
                    } else if (l_data.action === "game_with_bot") {
                        mainWindow.showShareWnd("vf");
                    } if (l_data.action === "rate_us") {
                        mainWindow.showFeedbackWnd();
                    } else if (l_data.action === "private_challenge") {
                        mainWindow.gameCore.rejectChallenge(l_data.id);
                    }
                }
            }

            Text {
                id: bbBtn3
                color: "#A8F4F4"
                text: "Skip"
                font.underline: true
                visible: false
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 24 * mainWindow.ratioObject

                MouseArea {
                    id: maBtn3
                    anchors.fill: parent
                    onClicked: {
                        console.log("btn3 feedback action");
                        mainWindow.processEscape();
                        if (l_data.action === "game_with_player") {
                            mainWindow.showSearchWarlockWnd("f");
                        } else if (l_data.action === "game_with_bot") {
                            mainWindow.showSearchWarlockWnd("vf");
                        }
                    }
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

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

    function errorLinkActivated(link) {
        console.log("wnd_spell.onLinkActivated", link, l_data.type);
        if (l_data.type === 12) {
            // unstarted game
            var arr_lnk = link.split("/");
            console.log("wnd_spell.onLinkActivated", arr_lnk, l_data.id);
            mainWindow.gameCore.leaveBattle(l_data.id, arr_lnk[2]);
            mainWindow.processEscape();
        }
    }

    function initErrFields() {
        console.log("wnd_spell.initFields", JSON.stringify(mainWindow.gERROR));
        if (!mainWindow.gERROR || (!mainWindow.gERROR.spell && !mainWindow.gERROR.data && !(mainWindow.gERROR.type >= 7))) {
        return;
        }

        var spell_code = mainWindow.gERROR.spell;
        if (mainWindow.gERROR.type >= 8) { // 8, 9
            l_data = mainWindow.gERROR;
        } else {
            l_data = spell_code ? {} : mainWindow.gERROR.data;
        }

        SGU.cleanUpWindow(mainWindow.ratioObject);

        if (l_data && (l_data.action || (l_data.type && (l_data.type >= 8)))) {
            icon.visible = false;
            if (l_data.type && (l_data.type === 21)) {
                SGU.prepareTooMuchBotBattles(l_data, dict);
            } else if (l_data.type && (l_data.type === 20)) {
                SGU.prepareGameWithBot(l_data, dict);
            } else if (l_data.type && (l_data.type === 19)) {
                SGU.prepareGameWithPlayer(l_data, dict);
            } else if (l_data.type && (l_data.type === 18)) {
                SGU.prepareShareWnd(l_data, dict);
            } else if (l_data.type && (l_data.type === 17)) {
                SGU.preparePersonalChallenge(l_data, dict);
            } else if (l_data.type && (l_data.type === 16)) {
                SGU.prepareBattleHistory(l_data, dict);
            } else if (l_data.type && (l_data.type === 15)) {
                SGU.prepareUserScore(l_data, dict);
            } else if (l_data.type && (l_data.type === 14)) {
                SGU.prepareRateUs(l_data, dict);
            } else if (l_data.type && (l_data.type === 13)) {
                SGU.prepareFeedback(l_data, dict);
            } else if (l_data.type && (l_data.type === 1013)) {
                SGU.prepareNotReadyFunctionalMsg(l_data, dict);
            } else if (l_data.type && (l_data.type === 12)) {
                SGU.prepareDataType12(l_data, dict);
            } else if (l_data.type && (l_data.type === 11)) {
                SGU.prepareDataType11(l_data, dict);
            } else if (l_data.type && (l_data.type === 10)) {
                SGU.prepareDataType10(l_data, dict);
            } else if (l_data.type && (l_data.type === 9)) {
                SGU.prepareDataType9(l_data, dict);
            } else if (l_data.type && (l_data.type === 8)) {
                SGU.prepareDataType8(l_data, dict);
            } else if (l_data.action === 'm') {
                SGU.prepareMonsterDetails(l_data, dict);
            } else if (l_data.action === 'hp') {
                SGU.prepareHPDetails(l_data, dict);
            }
            ltError.font.pixelSize = 28 * mainWindow.ratioFont;
        } else if (spell_code) {
            var spell_icon = BGU.map_spell_to_icon[spell_code];
            SGU.prepareSpellDescription(spell_code, spell_icon);
        }

        mainWindow.gERROR = {};
        console.log("wnd_spell.initFields", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
