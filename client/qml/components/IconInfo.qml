import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property alias text_color: btnText.color
    property alias text: btnText.text
    property alias fontSizeMode: btnText.fontSizeMode
    property alias source: iIcon.source
    property alias iconVisible: iIcon.visible
    property alias textVisible: btnText.visible

    property var l_data: ({})

    signal clicked
    signal doubleClicked

    color: "#551470"
    radius: 10

    Image {
        id: iIcon
        anchors.centerIn: parent;
        height: 0.9 * parent.height
        width: 0.9 * parent.height
    }

    LargeText {
        id: btnText
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 0.3 * parent.height
        width: 0.3 * parent.width
        color: "white"
        fontSizeMode: Text.Fit
    }

    SequentialAnimation on color {
        id: saClick
        loops: 1
        running: false

        ColorAnimation {
            from: "#551470"
            to: "white"
            duration: 200
        }
        ColorAnimation {
            from: "white"
            to: "#551470"
            duration: 200
        }
    }

    //Mouse area to react on click events
    MouseArea {
        anchors.fill: button
        //pressAndHoldInterval: 500
        onClicked: {
            if (active) {
                saClick.loops = 1;
                saClick.start();
                button.clicked();
            }
        }
        /*onDoubleClicked: {
            if (active) {
                saClick.loops = 2;
                saClick.start();
                button.doubleClicked();
            }
        }*/
        onPressAndHold: {
            //if (active) {
                saClick.loops = 2;
                saClick.start();
                button.doubleClicked();
            //}
        }
    }

    function onFinishCreation() {
        console.log("IconInfo.onFinishCreation");
    }

    Component.onCompleted: onFinishCreation();
}
