import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"

Item {
    id: rWarlock

    property var l_warlock: ({})
    property var l_IconInfoObj: ({})


    LargeText {
       id: ltName
       text: l_warlock.name
       anchors.top: parent.top
       anchors.horizontalCenter: parent.horizontalCenter
       height: 0.1 * parent.height
       width: 0.3 * parent.width
       bg_visible: true
       bg_color: l_warlock.player ? "#210430" : "#544653"
       color: l_warlock.player ? "#A8F4F4" : "#FEE2D6"
       border_visible: false
       radius: 10
    }

    IconInfo {
        id: iiHP
        source: "qrc:/res/heart.png";
        text: l_warlock.hp
        height: 0.10 * parent.height
        width: height
        anchors.top: parent.top
        //anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
    }

    IconInfo {
        id: iiBanked
        source: "qrc:/res/banked_spell.png";
        text: ""
        height: 0.10 * parent.height
        width: height
        anchors.top: parent.top
        //anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        visible: l_warlock.player && (l_warlock.banked_spell !== "")
    }

    ScrollView {
        id: svMonsters
        anchors.top: iiHP.bottom
        anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
        width: 0.5 * parent.width
        height: 0.15 * parent.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        Item {
            id: iMonsters
            anchors.fill: parent
        }
    }

    ScrollView {
        id: svCharm
        anchors.top: iiHP.bottom
        anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        width: 0.5 * parent.width
        height: 0.15 * parent.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        Item {
            id: iCharm
            anchors.fill: parent
        }
    }

    ScrollView {
        id: svGestures
        anchors.bottom: rBottomLine.top
        anchors.left: parent.left
        anchors.right: iiLeft.left
        height: 0.73 * rWarlock.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        ListView {
            id: lwGestures
            anchors.fill: parent
            orientation: ListView.Horizontal
            model: l_warlock.print_g
            delegate: Item {
                id: lwiGesture
                height: svGestures.height
                width: iiLeft.width

                IconInfo {
                    id: iiHL
                    source: "qrc:/res/"+lwGestures.model[index].l+".png"
                    iconVisible: lwGestures.model[index].lv
                    textVisible: false
                    height: 0.3 * rWarlock.height
                    width: parent.width
                    anchors.bottom: iiHR.top
                    anchors.bottomMargin: 0.01 * rWarlock.height
                    anchors.right: parent.right
                    active: false
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: "#E7FFFF" }
                            GradientStop { position: 1.0; color: "#FEE2D6" }
                        }
                }

                IconInfo {
                    id: iiHR
                    source: "qrc:/res/"+lwGestures.model[index].r+".png"
                    iconVisible: lwGestures.model[index].rv
                    textVisible: false
                    height: 0.3 * rWarlock.height
                    width: parent.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0.05 * rWarlock.height
                    anchors.right: parent.right
                    active: false
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: "#E7FFFF" }
                            GradientStop { position: 1.0; color: "#FEE2D6" }
                        }
                }
            }
        }
    }

    IconInfoR {
        id: iiLeft
        source: "qrc:/res/stars_light.png";
        text: ""
        height: 0.35 * parent.height
        width: 0.7 * height
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 0.01 * parent.height
        anchors.right: parent.right
        visible: l_warlock.player

        onClicked: {
            mainWindow.showGesture(true, "");
        }
    }

    IconInfoR {
        id: iiRight
        source: "qrc:/res/stars_light.png";
        text: ""
        height: iiLeft.height
        width: iiLeft.width
        anchors.bottom: rBottomLine.top
        //anchors.bottomMargin: 0.02 * parent.height
        anchors.right: parent.right
        visible: l_warlock.player
        onClicked: {
            mainWindow.showGesture(false, "");
        }
    }

    Rectangle {
        id: rBottomLine
        height: 0.03 * parent.height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        color: "#FEE2D6"
    }



    Component.onCompleted: finishWarlockCreation();

    function iconClick(data) {
        console.log("iconClick", l_warlock.name, JSON.stringify(data));
    }

    function iconDoubleClick(data) {
        console.log("iconDoubleClick", l_warlock.name, JSON.stringify(data));
    }

    function scrollGestures() {
        if (!svGestures.ScrollBar || !svGestures.ScrollBar.horizontal) {
            return;
        }

        console.log("Warlock.qml.finishWarlockCreation", svGestures.ScrollBar.horizontal.position, svGestures.ScrollBar.horizontal.stepSize);
        var cnt = 0;
        svGestures.ScrollBar.horizontal.stepSize = 0.1;
        while(svGestures.ScrollBar.horizontal.position < 1) {
            svGestures.ScrollBar.horizontal.increase();
            if (++cnt >= 10) {
                break;
            }
        }
        console.log("Warlock.qml.finishWarlockCreation", svGestures.ScrollBar.horizontal.position, cnt);
    }

    function prepareDynamic(arr, parent, code_value, start_x, incerment) {
        if (l_IconInfoObj.status === Component.Error || l_IconInfoObj.status !== Component.Ready) {
            console.log("prepareDynamic Error loading component:", l_IconInfoObj.errorString());
            return ;
        }
        var curr_x = start_x;
        var total_width = 0;
        for(var i = 0, Ln = arr.length; i < Ln; ++i) {
            if (!arr[i]) {
                continue;
            }
            var arr_m = arr[i];
            var sprite = l_IconInfoObj.createObject(parent, {l_data: arr_m,x: curr_x, y: 0, height: parent.height, width: parent.height, text: arr_m[code_value], source: "qrc:/res/"+arr_m.icon+".png"});
            if (sprite === null) {
                console.log("prepareDynamic Error creating object");
                continue;
            }
            console.log("prepareDynamic looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
            sprite.clicked.connect(function () { rWarlock.iconClick(arr_m); });
            sprite.doubleClicked.connect(function () { rWarlock.iconDoubleClick(arr_m); });

            curr_x += incerment * sprite.width;
            total_width += sprite.width;
        }
    }

    function prepareMonsters() {
        prepareDynamic(l_warlock.monsters, iMonsters, "hp", 0, 1);
    }

    function prepareState() {
        prepareDynamic(l_warlock.statusIcons, iCharm, "value", iCharm.width - iCharm.height, -1);
    }


    function finishWarlockCreation() {
        console.log("finishWarlockCreation", JSON.stringify(l_warlock));
        scrollGestures();
        prepareMonsters();
        prepareState();
    }
}
