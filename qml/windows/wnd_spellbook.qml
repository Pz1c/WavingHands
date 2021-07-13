// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    //icon.visible: true
    //icon.source: "qrc:/res/spellbook.png"
    //title.visible: true
    //title.text: dict.getStringByCode("Spellbook")

    property var arrSpellInit: [{g:'Disruptions:',t:1,n:''},{g:'DPP',t:0,n:''},{g:'DSF',t:0,n:''},{g:'FFF',t:0,n:''},{g:'SWD',t:0,n:''},{g:'PSDD',t:0,n:''},{g:'PSDF',t:0,n:''},{g:'SPFP',t:0,n:''},
        {g:'Damaging:',t:1,n:''},{g:'>',t:0,n:''},{g:'SD',t:0,n:''},{g:'WFP',t:0,n:''},{g:'WPFD',t:0,n:''},{g:'WDDc',t:0,n:''},{g:'DFFDD',t:0,n:''},{g:'FSSDD',t:0,n:''},{g:'PWPFSSSD',t:0,n:''},
        {g:'Healing:',t:1,n:''},{g:'DFW',t:0,n:''},{g:'DFPW',t:0,n:''},
        {g:'Monsters:',t:1,n:''},{g:'SFW',t:0,n:''},{g:'PSFW',t:0,n:''},{g:'FPSFW',t:0,n:''},{g:'WFPSFW',t:0,n:''},
        {g:'Protection:',t:1,n:''},{g:'P',t:0,n:''},{g:'p',t:0,n:''},{g:'WWP',t:0,n:''},
        {g:'Counters:',t:1,n:''},{g:'cw',t:0,n:''},{g:'cDPW',t:0,n:''},{g:'WPP',t:0,n:''},{g:'WWS',t:0,n:''},{g:'PDWP',t:0,n:''},
        {g:'Advanced Enchantments:',t:1,n:''},{g:'PPws',t:0,n:''},{g:'DWFFd',t:0,n:''},{g:'DWFWd',t:0,n:''},{g:'SPPc',t:0,n:''},{g:'SPPFD',t:0,n:''},{g:'PWPWWc',t:0,n:''},{g:'DSFFFc',t:0,n:''},{g:'DWWFWD',t:0,n:''},{g:'DWSSSP',t:0,n:''},{g:'SPFPSDW',t:0,n:''},
        {g:'Elements:',t:1,n:''},{g:'SSFP',t:0,n:''},{g:'WWFP',t:0,n:''},{g:'WSSc',t:0,n:''},{g:'SWWc',t:0,n:''},{g:'cSWWS',t:0,n:''},{g:'cWSSW',t:0,n:''}]
    property var arrSpell: []

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        Text {
            id: ttTitle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 0.05 * parent.width
            anchors.right: parent.right
            font.pixelSize: 42 * mainWindow.ratioFont
            color: "#10C9F5"
            horizontalAlignment: Text.AlignLeft
            text: dict.getStringByCode("Spellbook")
        }

        ScrollView {
            id: svError
            anchors.top: ttTitle.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            contentWidth: -1
            z:13

            ListView {
                id: lvSpellList
                model: []
                anchors.fill: parent

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
                        width: 0.95 * parent.width
                        anchors.bottomMargin: 0.01 * dialogWindow.height

                        Text {
                            id: rdbiGesture
                            anchors.top: rdSpellItem.top
                            anchors.topMargin: lvSpellList.model[index].t ? 10 * mainWindow.ratioObject : 0
                            anchors.bottom: rdSpellItem.bottom
                            anchors.left: rdSpellItem.left
                            anchors.leftMargin: 0.03 * parent.width
                            font.pixelSize: (lvSpellList.model[index].t ? 21 : 28) * mainWindow.ratioFont
                            color: lvSpellList.model[index].t ? "#10C9F5" : "#FEE2D6"
                            horizontalAlignment: Text.AlignLeft
                            text: lvSpellList.model[index].g
                        }

                        Text {
                            id: rdbifTitle
                            anchors.verticalCenter: rdSpellItem.verticalCenter
                            anchors.right: parent.right//rdbifInfo.left
                            anchors.rightMargin: 0.03 * parent.width
                            height: 0.8 * rdSpellItem.height
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#FEE2D6"
                            fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignRight
                            text: lvSpellList.model[index].n
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                console.log("choose spell", mainWindow.gBattle.spellIdx, index, JSON.stringify(lvSpellList.model[index]), JSON.stringify(arrSpell[index]));
                                if (!lvSpellList.model[index].t) {
                                    mainWindow.showSpellDetails(lvSpellList.model[index].g);
                                }
                            }
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
        initGFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initGFields() {
        console.log("wnd_spellbook.initGFields", arrSpell.length);

        if (arrSpell.length === 0) {
            arrSpell = arrSpellInit;
            for (var i = 0, Ln = arrSpell.length; i < Ln; ++i) {
                if (arrSpell[i].t === 0) {
                    arrSpell[i].n = dict.getStringByCode(arrSpell[i].g);
                }
            }
        }
        lvSpellList.model = arrSpell;
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
