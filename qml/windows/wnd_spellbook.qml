
// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

import "qrc:/qml/components"
import "qrc:/js/battle_gui_utils.js" as BGU


InfoWindow {
    id: dMainItem

    //icon.visible: true
    //icon.source: "qrc:/res/spellbook.png"
    //title.visible: true
    //title.text: dict.getStringByCode("Spellbook")


    property var arrSpellInit: [ {t:4,sbl:0}, {t:2,sbl:0}, {t:3,sbl:0},
        {g:'Disruptions:',t:1,n:'',sbl:1},
                {g:'DPP',t:0,n:'',sbl:1,st:1},{g:'DSF',t:0,n:'',sbl:1,st:1},{g:'FFF',t:0,n:'',sbl:2,st:1},{g:'SWD',t:0,n:'',sbl:1,st:1},
                {g:'PSDD',t:0,n:'',sbl:2,st:1},{g:'PSDF',t:0,n:'',sbl:2,st:1},{g:'SPFP',t:0,n:'',sbl:3,st:1},
        {g:'Damaging:',t:1,n:'',sbl:1},
                {g:'>',t:0,n:'',sbl:1,st:2},{g:'SD',t:0,n:'',sbl:1,st:2},{g:'WFP',t:0,n:'',sbl:1,st:2},{g:'WPFD',t:0,n:'',sbl:1,st:2},
                {g:'WDDc',t:0,n:'',sbl:3,st:2},{g:'DFFDD',t:0,n:'',sbl:3,st:2},{g:'FSSDD',t:0,n:'',sbl:3,st:2},{g:'PWPFSSSD',t:0,n:'',sbl:5,st:2},
        {g:'Healing:',t:1,n:'',sbl:1},
                {g:'DFW',t:0,n:'',sbl:1,st:3},{g:'DFPW',t:0,n:'',sbl:2,st:3},
        {g:'Monsters:',t:1,n:'',sbl:1},
                {g:'SFW',t:0,n:'',sbl:1,st:4},{g:'PSFW',t:0,n:'',sbl:2,st:4},{g:'FPSFW',t:0,n:'',sbl:2,st:4},{g:'WFPSFW',t:0,n:'',sbl:2,st:4},
        {g:'Protection:',t:1,n:'',sbl:1},
                {g:'P',t:0,n:'',sbl:1,st:5},{g:'p',t:0,n:'',sbl:1,st:5},{g:'WWP',t:0,n:'',sbl:1,st:5},
        {g:'Counters:',t:1,n:'',sbl:2},
                {g:'cw',t:0,n:'',sbl:2,st:6},{g:'cDPW',t:0,n:'',sbl:3,st:6},{g:'WPP',t:0,n:'',sbl:2,st:6},
                {g:'WWS',t:0,n:'',sbl:2,st:6},{g:'PDWP',t:0,n:'',sbl:3,st:6},
        {g:'Advanced Enchantments:',t:1,n:'',sbl:3},
                {g:'PPws',t:0,n:'',sbl:3,st:7},{g:'DWFFd',t:0,n:'',sbl:3,st:7},{g:'DWFWd',t:0,n:'',sbl:3,st:7},
                {g:'SPPc',t:0,n:'',sbl:5},{g:'SPPFD',t:0,n:'',sbl:5,st:7},{g:'PWPWWc',t:0,n:'',sbl:5,st:7},
                {g:'DSFFFc',t:0,n:'',sbl:3,st:7},{g:'DWWFWD',t:0,n:'',sbl:5,st:7},{g:'DWSSSP',t:0,n:'',sbl:5,st:7},
                {g:'SPFPSDW',t:0,n:'',sbl:5,st:7},
        {g:'Elements:',t:1,n:'',sbl:4},
                {g:'SSFP',t:0,n:'',sbl:4,st:8},{g:'WWFP',t:0,n:'',sbl:4,st:8},{g:'WSSc',t:0,n:'',sbl:4,st:8},
                {g:'SWWc',t:0,n:'',sbl:4,st:8},{g:'cSWWS',t:0,n:'',sbl:4,st:8},{g:'cWSSW',t:0,n:'',sbl:4,st:8}
        ]
    property var arrSpell: []
    property int lastSpellbookLevel: 1
    property string levelUpHint: ""

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        ScrollView {
            id: svError
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            //anchors.fill: parent.fill
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            contentWidth: -1
            z: 13

            ListView {
                id: lvSpellList
                model: []
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 0.05 * parent.width
                anchors.right: parent.right
                anchors.rightMargin: 0.05 * parent.width
                //height: contentHeight
                width: dialogWindow.width
                delegate: chooser

                DelegateChooser {
                    id: chooser
                    role: "t"
                    DelegateChoice {
                        roleValue: "0"
                        delegate: Item {
                            id: idRoot
                            width: lvSpellList.width
                            height: 0.05 * dialogWindow.height

                            Rectangle {
                                id: rdSpellItem
                                color: "transparent"
                                radius: 30
                                anchors.centerIn: parent
                                height: 0.95 * parent.height
                                width: parent.width
                                anchors.bottomMargin: 0.01 * dialogWindow.height

                                Text {
                                    id: rdbiGesture
                                    anchors.top: rdSpellItem.top
                                    anchors.topMargin: 0
                                    anchors.bottom: rdSpellItem.bottom
                                    anchors.left: rdSpellItem.left
                                    //anchors.leftMargin: 0.03 * parent.width
                                    font.pixelSize: 21 * mainWindow.ratioFont
                                    color: "#FEE2D6"
                                    horizontalAlignment: Text.AlignLeft
                                    text: lvSpellList.model[index].g
                                }

                                Text {
                                    id: rdbifTitle
                                    anchors.verticalCenter: rdSpellItem.verticalCenter
                                    anchors.right: rSpellIcon.left //rdbifInfo.left
                                    anchors.rightMargin: 0.03 * parent.width
                                    height: 0.8 * rdSpellItem.height
                                    font.pixelSize: 28 * mainWindow.ratioFont
                                    color: "#FEE2D6"
                                    fontSizeMode: Text.VerticalFit
                                    horizontalAlignment: Text.AlignRight
                                    text: lvSpellList.model[index].n
                                }

                                Rectangle {
                                    id: rSpellIcon
                                    height: 0.9 * parent.height
                                    width: height
                                    anchors.verticalCenter: rdSpellItem.verticalCenter
                                    anchors.right: parent.right
                                    radius: 10
                                    color: (lvSpellList.model[index].st === 3 ? "#0654C0" : (lvSpellList.model[index].st === 2 ? "#FEE2D6" : "#551470"))

                                    Image {
                                        id: iSpellIcon
                                        source: BGU.getFullSpellIconByGesture(lvSpellList.model[index].g)
                                        anchors.fill: parent
                                    }
                                }

                                MouseArea {
                                    id: maSpell
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("choose spell",
                                                    mainWindow.gBattle.spellIdx,
                                                    index, JSON.stringify(
                                                        lvSpellList.model[index]),
                                                    JSON.stringify(
                                                        arrSpell[index]))
                                        mainWindow.showSpellDetails(
                                                    lvSpellList.model[index].g)
                                    }
                                }
                            }
                        }
                    }

                    DelegateChoice {
                        roleValue: "1"
                        delegate: Item {
                            id: idRoot1
                            width: lvSpellList.width
                            height: 0.05 * dialogWindow.height

                            Rectangle {
                                id: rdSpellItem1
                                color: "transparent"
                                radius: 30
                                anchors.centerIn: parent
                                height: 0.95 * parent.height
                                width: parent.width
                                anchors.topMargin: 0.04 * dialogWindow.height
                                anchors.bottomMargin: 0.01 * dialogWindow.height

                                Text {
                                    id: rdbiGesture1
                                    anchors.top: rdSpellItem1.top
                                    anchors.topMargin: 0
                                    anchors.bottom: rdSpellItem1.bottom
                                    anchors.left: rdSpellItem1.left
                                    //anchors.leftMargin: 0.03 * parent.width
                                    font.pixelSize: 28 * mainWindow.ratioFont
                                    color: "#10C9F5"
                                    horizontalAlignment: Text.AlignLeft
                                    text: lvSpellList.model[index].g
                                }
                            }
                        }
                    }

                    DelegateChoice {
                        roleValue: "2"
                        delegate: Item {
                            id: d2
                            width: 0.8 * dialogWindow.width
                            height: 1.5 * ttTitle.contentHeight
                            Text {
                            id: ttTitle
                            font.pixelSize: 42 * mainWindow.ratioFont
                            color: "#10C9F5"
                            horizontalAlignment: Text.AlignLeft
                            wrapMode: Text.Wrap
                            anchors.fill: parent
                            text: dict.getStringByCode("SpellbookName" + lastSpellbookLevel)
                        }
                        }
                    }

                    DelegateChoice {
                        roleValue: "3"
                        delegate: Item {
                            id: d3
                            width: 0.8 * dialogWindow.width
                            height: 1.5 * ttSubTitle.contentHeight
                            Text {
                            id: ttSubTitle
                            font.pixelSize: 21 * mainWindow.ratioFont
                            color: "#FEE2D6"
                            horizontalAlignment: Text.AlignLeft
                            text: dict.getStringByCode("SpellbookSubTitle")
                            wrapMode: Text.Wrap
                            anchors.fill: parent
                        } }
                    }

                    DelegateChoice {
                        roleValue: "4"
                        delegate: Item {
                            id: d4
                            width: dialogWindow.width
                            height: 1.4 * rHint.height

                            Rectangle {
                            id: rHint
                            color: "transparent"
                            anchors.top: parent.top
                            anchors.left: parent.left
                            border.color: "#10C9F5"
                            border.width: 2 * mainWindow.ratioObject
                            height: 1.4 * ttHint.contentHeight
                            width: 0.9 * dialogWindow.width

                            Item {
                                id: iHint
                                anchors.centerIn: parent
                                width: 0.6 * dialogWindow.width
                                height: ttHint.contentHeight

                                Text {
                                    id: ttHint
                                    anchors.fill: parent
                                    font.pixelSize: 21 * mainWindow.ratioFont
                                    color: "#10C9F5"
                                    horizontalAlignment: Text.AlignLeft
                                    wrapMode: Text.Wrap
                                    text: levelUpHint
                                }
                            }
                        } }
                    }
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape()
    }

    function showWnd() {
        initGFields()
        visible = true
    }

    function hideWnd() {
        visible = false
    }

    function initGFields() {
        console.log("wnd_spellbook.initGFields", arrSpell.length,
                    mainWindow.playerSpellbookLevel, lastSpellbookLevel, JSON.stringify(mainWindow.gERROR))
        var sbl = mainWindow.gERROR.sbl;
        var win_vs_bot = mainWindow.gERROR.win_vs_bot;
        levelUpHint = mainWindow.gERROR.lelveup_hint;
        mainWindow.gERROR = {};

        if ((arrSpell.length === 0)
                || (lastSpellbookLevel !== sbl)) {
            arrSpell = [];
            for (var i = 0, Ln = arrSpellInit.length; i < Ln; ++i) {
                if ((arrSpellInit[i].t === 0) && (arrSpellInit[i].n === '')) {
                    arrSpellInit[i].n = dict.getStringByCode(arrSpellInit[i].g)
                }
                if ((arrSpellInit[i].sbl <= sbl) &&
                        !((arrSpellInit[i].t === 4) && (sbl === 5))) {
                    arrSpell.push(arrSpellInit[i]);
                }
            }
        }
        lastSpellbookLevel = sbl;
        if ((lastSpellbookLevel === 3) && (win_vs_bot === 2)) {
            levelUpHint = dict.getStringByCode("SpellbookHint3.1");
        } else {
            levelUpHint = dict.getStringByCode("SpellbookHint" + sbl);
        }

        lvSpellList.model = arrSpell
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem)
        initGFields()
    }
}
