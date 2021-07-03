// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/battle_gui_utils.js" as BGU

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
                text: "Details:"
                horizontalAlignment: Text.AlignLeft
                //fontSizeMode: Text.VerticalFit
            }

            Text {
                id: ltError
                anchors.top: ltDesc.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: 21 * mainWindow.ratioFont
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignJustify
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: dict.getStringByCode("MonsterSetTarget")
                bg_color_active: "#551470"
                border_color_active: "#551470"
                radius: 30
                visible: false
                z: 30

                gradient: Gradient {
                    GradientStop { position: 0.0 ; color: "#905B93" }
                    GradientStop { position: 0.75; color: "#551470" }
                    GradientStop { position: 1.0 ; color: "#551470" }
                }

                width: 366 * ratioObject
                height: 96 * ratioObject
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 144 * ratioObject
                anchors.horizontalCenter: parent.horizontalCenter


                onClicked: {
                    console.log("chooseMonsterTarget", JSON.stringify(l_data));
                    if (l_data.under_control) {
                        mainWindow.chooseMonsterTarget(ltTitle.text);
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

    function initErrFields() {
        console.log("wnd_spell.initFields", JSON.stringify(mainWindow.gERROR));
        if (!mainWindow.gERROR || (!mainWindow.gERROR.spell && !mainWindow.gERROR.data)) {
            return;
        }
        var spell_code = mainWindow.gERROR.spell;
        l_data = mainWindow.gERROR.data
        bbAction.visible = false;
        if (l_data) {
          if (l_data.action === 'm') {
            icon.source = "qrc:/res/" + l_data.icon + ".png";
            icon.visible = true;
            icon.text = l_data.hp;
            var details = "Life: Unknown<br>Power: Unknown<br>Owner: " + l_data.owner + "<br>Target: " + l_data.target;
            if (l_data.hp === "R") {
              ltTitle.text = dict.getStringByCode("RH_summon");
              ltShortDesc.text = dict.getStringByCode("WH_summon");
            } else if (l_data.hp === "L") {
                ltTitle.text = dict.getStringByCode("LH_summon");
                ltShortDesc.text = dict.getStringByCode("WH_summon");
            } else {
              ltTitle.text = l_data.name;
              ltShortDesc.text = dict.getStringByCode("Warlock");
              details = "Life: " + l_data.hp + "<br>Power: "+l_data.strength+"<br>Owner: " + l_data.owner + "<br>Target: " + l_data.target;
            }
            ltError.text = details;
            bbAction.visible = l_data.under_control;
          } else if (l_data.action === 'hp') {
            icon.source = "qrc:/res/heart_small.png";
            icon.text = l_data.value;
            icon.visible = true;
            ltTitle.text = l_data.warlock_name;
            ltShortDesc.text = dict.getStringByCode("Warlock");
            ltError.text = "Life: " + l_data.value;
          }
          ltError.font.pixelSize = 28 * mainWindow.ratioFont;
        } else if (spell_code) {
            var spell_icon = BGU.map_spell_to_icon[spell_code];
            if (spell_icon) {
                icon.source = "qrc:/res/"+spell_icon+".png";
                icon.visible = true;
            } else {
                icon.visible = false;
            }

            ltTitle.text = dict.getStringByCode(spell_code);
            ltShortDesc.text = dict.getStringByCode(spell_code + "_short_desc");
            ltError.font.pixelSize = 21 * mainWindow.ratioFont;
            ltError.text = dict.getStringByCode(spell_code + "_desc");
        }

        mainWindow.gERROR = {};
        console.log("wnd_spell.initFields", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
