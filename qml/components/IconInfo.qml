import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool animationEnabled: true
    property bool checkbox: false
    property bool checked: false
    property bool blink: false
    property bool disbleBlinkAfterClick: true
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias source: iIcon.source
    property alias iconVisible: iIcon.visible
    property alias textVisible: btnText.visible

    property var l_data: ({})

    signal clicked(var data)
    signal doubleClicked(var data)

    color: "#551470"
    radius: 10
    border.color: "snow"
    border.width: 0

    Image {
        id: iIcon
        anchors.centerIn: button;
        height: 0.9 * button.height
        width: 0.9 * button.height
    }

    LargeText {
        id: btnText
        anchors.right: button.right
        anchors.bottom: button.bottom
        height: 0.3 * button.height
        width: 0.3 * button.width
        color: "white"
        fontSizeMode: Text.Fit
    }

    SequentialAnimation on color {
        id: saClick
        loops: Animation.Infinite
        running: false

        ColorAnimation {
            from: button.color
            to: "snow"
            duration: 200
        }
        ColorAnimation {
            from: "snow"
            to: button.color
            duration: 200
        }
    }

    //Mouse area to react on click events
    MouseArea {
        anchors.fill: button
        //pressAndHoldInterval: 500
        onClicked: {
            if (active) {
                if (blink && disbleBlinkAfterClick) {
                    blink = false;
                    saClick.stop();
                }

                animate(1);
                if (checkbox) {
                    checked = !checked;
                    button.border.width = checked ? 3 : 0;
                    l_data.checked = checked;
                }

                button.clicked(l_data);
            }
        }
        /*onDoubleClicked: {
            if (active) {
                animate(2);
                button.doubleClicked();
            }
        }*/
        onPressAndHold: {
            //if (active) {
                animate(2);
                button.doubleClicked(l_data);
            //}
        }
    }

    function animate(cnt) {
        console.log("IconInfo.animate", cnt);
        if (cnt === -1) {
            saClick.stop();
            return ;
        }

        if (animationEnabled){
            if (cnt) {
                saClick.loops = cnt;
            }
            saClick.start();
        }
    }

    function onFinishCreation() {
        console.log("IconInfo.onFinishCreation", JSON.stringify(l_data));
        //if (l_data.action)
        /*if (blink) {
            saClick.start();
        }*/
    }

    Component.onCompleted: onFinishCreation();
}
