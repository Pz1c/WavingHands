
// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    //icon.visible: true
    //icon.source: "qrc:/res/spellbook.png"
    //title.visible: true
    //title.text: dict.getStringByCode("Spellbook")
    property var modelAll: [];
    property var modelActive: [];

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#210430" }
        GradientStop { position: 1.0; color: "#2DA0A5" }
    }

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        Text {
            id: topTitle
            anchors.top: parent.top
            anchors.topMargin: 5 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            font.pixelSize: 49 * mainWindow.ratioFont
            color: "#E7FFFF"
            horizontalAlignment: Text.AlignLeft
            text: "Hall of Fame"
        }

        Rectangle {
            id: topTitleLine
            anchors.top: topTitle.bottom
            anchors.topMargin: 5 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#E7FFFF"
            height: 3 * mainWindow.ratioObject
        }

        Item {
            id: topSwitch
            anchors.top: topTitleLine.bottom
            anchors.topMargin: 20 * mainWindow.ratioObject
            anchors.horizontalCenter: parent.horizontalCenter
            //anchors.left: parent.left
            //anchors.right: parent.right
            height: 110 * mainWindow.ratioObject
            width: 0.6 * parent.width
            z: 15

            Rectangle {
                id: topSwitchActive

                color: "#A8F4F4"
                border.color: "#A8F4F4"
                border.width: 3 * mainWindow.ratioObject
                radius: 15 * mainWindow.ratioObject
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 45 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: -15 * mainWindow.ratioObject
                z: 20

                Text {
                    id: topSwitchActiveText
                    anchors.centerIn: parent
                    font.pixelSize: 28 * mainWindow.ratioFont
                    color: "#210430"
                    text: "Active"
                }

                MouseArea {
                    id: topMaAll
                    anchors.fill: parent
                    onClicked: {
                        switchState();
                    }
                }
            }

            Rectangle {
                id: topSwitchAll

                color: "#210430"
                border.color: "#A8F4F4"
                border.width: 3 * mainWindow.ratioObject
                radius: 15 * mainWindow.ratioObject
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 45 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: -15 * mainWindow.ratioObject
                z: 17

                Text {
                    id: topSwitchAllText
                    anchors.centerIn: parent
                    font.pixelSize: 28 * mainWindow.ratioFont
                    color: "#A8F4F4"
                    text: "All time"
                }

                MouseArea {
                    id: topMaActive
                    anchors.fill: parent
                    onClicked: {
                        switchState();
                    }
                }
            }

            Text {
                id: topSwitchBottomTitle
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 5 * mainWindow.ratioObject
                font.pixelSize: 21 * mainWindow.ratioFont
                color: "#E7FFFF"
                horizontalAlignment: Text.AlignLeft
                text: "Players active in the last 3 days:"
            }
        }

        ScrollView {
            id: svError
            anchors.top: topSwitch.bottom
            anchors.topMargin: 5 * mainWindow.ratioObject
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
                delegate: Item {
                    id: idRoot
                    width: lvSpellList.width
                    height: dMainItem.rowHeight

                    Rectangle {
                        id: rdTopItem
                        color: "transparent"
                        radius: 30
                        anchors.centerIn: parent
                        height: 0.95 * parent.height
                        width: parent.width
                        anchors.bottomMargin: 0.01 * dialogWindow.height

                        Text {
                            id: rdbiIndex
                            //anchors.top: rdTopItem.top
                            //anchors.topMargin: 0
                            //anchors.bottom: rdTopItem.bottom
                            anchors.verticalCenter: rdTopItem.verticalCenter
                            anchors.left: rdTopItem.left
                            anchors.leftMargin: 20 * mainWindow.ratioObject
                            width: 50 * mainWindow.ratioObject
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#210430"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: (index + 1) + "."
                        }

                        Rectangle {
                            id: rdbiBG
                            anchors.verticalCenter: rdTopItem.verticalCenter
                            anchors.left: rdbiIndex.right //rdbifInfo.left
                            anchors.leftMargin: 10 * mainWindow.ratioObject
                            anchors.right: parent.right //rdbifInfo.left
                            anchors.rightMargin: 10 * mainWindow.ratioObject
                            height: 0.9 * parent.height
                            radius: 20
                            color: "#2DA0A5"
                            opacity: 0.4
                        }

                        Text {
                            id: rdbifTitle
                            anchors.verticalCenter: rdTopItem.verticalCenter
                            anchors.left: rdbiIndex.right //rdbifInfo.left
                            anchors.leftMargin: 30 * mainWindow.ratioObject
                            height: 0.8 * rdTopItem.height
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#A8F4F4"
                            //fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            text: lvSpellList.model[index].n
                            //font.underline: lvSpellList.model[index].n.toLower()
                        }

                        Text {
                            id: rdbifOnline
                            anchors.verticalCenter: rdTopItem.verticalCenter
                            anchors.left: rdbifTitle.right
                            anchors.leftMargin: 10 * mainWindow.ratioObject
                            height: 0.8 * rdTopItem.height
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#FEE2D6"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            text: "online"
                            font.italic: true
                            visible: lvSpellList.model[index].a
                        }

                        Text {
                            id: rdbifElo
                            anchors.verticalCenter: rdTopItem.verticalCenter
                            anchors.right: parent.right //rdbifInfo.left
                            anchors.rightMargin: 30 * mainWindow.ratioObject
                            height: 0.8 * rdTopItem.height
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#E7FFFF"
                            //fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            text: lvSpellList.model[index].e
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                var str = JSON.stringify(lvSpellList.model[index]);
                                console.log("top click", str);
                                var obj = JSON.parse(str);
                                obj.name = obj.n;
                                obj.elo = obj.e;
                                obj.played = obj.p;
                                obj.won = obj.w;
                                obj.died = obj.d;
                                obj.lost = obj.l;
                                obj.last_activity = obj.la;
                                obj.online = obj.a;
                                obj.mobile = obj.mob
                                mainWindow.showUserScoreWnd(obj);
                            }
                        }
                    }
                }
            }
        }
    }

    // An open window must follow the roster too: main.qml only reacts to
    // topListChanged while it is waiting to open this window.
    Connections {
        target: mainWindow.gameCore
        function onTopListChanged() {
            // The hidden tab is not on screen: drop its cache so switchState()
            // reparses it on the next tap instead of showing an older roster.
            if (dMainItem.switchStateValue === 0) {
                dMainItem.modelAll = [];
            } else {
                dMainItem.modelActive = [];
            }
            dMainItem.refreshShownTab();
        }
    }

    Connections {
        target: lvSpellList
        function onMovementEnded() {
            if (dMainItem.refreshPending) {
                dMainItem.refreshPending = false;
                dMainItem.refreshShownTab();
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
        console.log("wnd_hall_of_fame.initGFields", JSON.stringify(mainWindow.gERROR))
        // The window is cached across close/reopen and the models are only
        // refetched when empty, so without this a refreshed roster is invisible.
        modelAll = [];
        modelActive = [];
        refreshPending = false;
        mainWindow.gERROR = {};
        switchState(0);
    }

    // A roster that cannot be parsed shows as an empty list instead of keeping
    // the window from opening.
    function loadTopList(tab) {
        try {
            var rows = JSON.parse(mainWindow.gameCore.getTopList(tab));
            return Array.isArray(rows) ? rows : [];
        } catch (e) {
            console.log("wnd_hall_of_fame.loadTopList", tab, e);
            return [];
        }
    }

    // Only what a row shows decides whether the list is rebuilt: the activity
    // timestamp and colour move on nearly every fetch for anyone online, and
    // neither is displayed nor used on click (showUserScoreWnd refetches the
    // profile by name).
    function rowsSig(rows) {
        var sig = [];
        for (var i = 0; i < rows.length; ++i) {
            sig.push(JSON.stringify([rows[i].n, rows[i].e, rows[i].a]));
        }
        return sig.join(",");
    }

    function refreshShownTab() {
        if (!visible) {
            return;
        }
        var tab = switchStateValue;
        var shown = tab === 0 ? modelActive : modelAll;
        var rows = loadTopList(tab);
        if (rowsSig(rows) === rowsSig(shown)) {
            return;
        }
        if (lvSpellList.moving) {
            // A rebuild stops a flick dead and cancels a press: wait for it to settle.
            refreshPending = true;
            return;
        }
        // Keep the player at the top of the view in place, not the pixel offset:
        // rows above them may have come, gone or moved.
        var y = lvSpellList.contentY;
        var top = Math.floor(y / rowHeight);
        if ((top >= 0) && (top < shown.length)) {
            for (var i = 0; i < rows.length; ++i) {
                if (rows[i].n === shown[top].n) {
                    y += (i - top) * rowHeight;
                    break;
                }
            }
        }
        if (tab === 0) {
            modelActive = rows;
        } else {
            modelAll = rows;
        }
        lvSpellList.model = rows;
        lvSpellList.forceLayout();
        lvSpellList.contentY = Math.max(0, Math.min(y, lvSpellList.contentHeight - lvSpellList.height));
    }

    function switchState(Val) {
        console.log("switchState", switchStateValue, Val);
        if ((Val >= 0) && (Val < 2)) {
            switchStateValue = Val;
        } else {
            switchStateValue = switchStateValue > 0 ? 0 : 1;
        }

        topSwitchBottomTitle.visible = switchStateValue === 0;
        switch(switchStateValue) {
        case 0:
            topSwitchActive.color = "#A8F4F4";
            topSwitchActiveText.color = "#210430";
            topSwitchActive.z = 20;
            topSwitchAll.color = "#210430";
            topSwitchAllText.color = "#A8F4F4";
            topSwitchAll.z = 17;
            if (modelActive.length == 0) {
                modelActive = loadTopList(0);
            }
            lvSpellList.model = modelActive;
            break;
        case 1:
            topSwitchAll.color = "#A8F4F4";
            topSwitchAllText.color = "#210430";
            topSwitchAll.z = 20;
            topSwitchActive.color = "#210430";
            topSwitchActiveText.color = "#A8F4F4";
            topSwitchActive.z = 17;
            if (modelAll.length == 0) {
                modelAll = loadTopList(1);
            }
            lvSpellList.model = modelAll;
            break;
        }
        ;
    }

    property int switchStateValue: 0
    // A roster change arrived while the list was moving; applied when it stops.
    property bool refreshPending: false
    // One row of the list; also what refreshShownTab scrolls by.
    readonly property real rowHeight: 100 * mainWindow.ratioObject
    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem)
        initGFields()
    }
}
