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
    property bool borderOnPress: true
    property bool buttonPressed: false
    property int borderOnPressWidth: 3
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias source: iIcon.source
    property alias iconVisible: iIcon.visible
    property alias textVisible: btnText.visible
    property alias iconHeight: iIcon.height
    property alias iconWidth: iIcon.width
    property alias textHeight: btnText.height
    property alias textWidth: btnText.width
    property alias textAnchors: btnText.anchors
    property alias iconAnchors: iIcon.anchors
    //property alias paddingLeft: anchors.leftMargin
    //property alias paddingRignt: anchors.rightMargin
    property string bg_color: "#551470"
    property string bg_color_checked: "#A8F4F4"

    property var l_data: ({})

    signal clicked(var data)
    signal doubleClicked(var data)

    color: bg_color
    radius: 10
    border.color: "snow"
    border.width: 0

    Image {
        id: iIcon
        anchors.centerIn: button;
        height: 1
        width: 1
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
            from: button.color;
            to: "snow"
            duration: 200
        }
        ColorAnimation {
            from: "snow"
            to: button.color;
            duration: 200
        }
        onRunningChanged: {
            if (!saClick.running && checkbox) {
                button.color = checked ? bg_color_checked : bg_color;
            }
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
                    setChecked(!checked);
                }

                button.clicked(l_data);
            }
        }

        property real borderBeforePress: 0;
        onPressed: {
            if (borderOnPress) {
                borderBeforePress = border.width;
                border.width = borderOnPressWidth;
            }
            buttonPressed = true;
        }

        onReleased: {
            if (borderOnPress) {
                border.width = borderBeforePress;
            }
            buttonPressed = false;
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

    function setChecked(NewChecked) {
        checked = NewChecked;
        button.border.width = checked ? 3 : 0;
        button.color = checked ? bg_color_checked : bg_color;
        l_data.checked = checked;
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
        //console.log("IconInfo.onFinishCreation", JSON.stringify(l_data), iIcon.height, button.height);
        if (iIcon.height <= 1) {
            var wh = Math.min(button.height, button.width);
            iIcon.height = 0.9 * wh;
            iIcon.width = 0.9 * wh;
        }
        if (l_data && l_data.hasOwnProperty("active")) {
          active = l_data.active;
        }
    }

    Component.onCompleted: onFinishCreation();
}
