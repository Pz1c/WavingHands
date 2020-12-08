import QtQuick 2.9

//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool animationEnabled: true
    property bool checkbox: false
    property bool checked: false
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
                animate(1);
                button.clicked(l_data);
                if (checkbox) {
                    checked = !checked;
                    button.border.width = checked ? 3 : 0;
                }
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
        if (animationEnabled){
            saClick.loops = cnt;
            saClick.start();
        }
    }

    function onFinishCreation() {
        console.log("IconInfo.onFinishCreation");
    }

    Component.onCompleted: onFinishCreation();
}
