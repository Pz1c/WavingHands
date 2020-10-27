import QtQuick 2.9
import "qrc:/js/text_utils.js" as TU
//Implementation of the Button control.
Rectangle {
    id: button

    property bool active: true
    property bool transparent: false
    property alias image: btnIcon.source
    property alias text_color: btnText.color
    property int value: 0
    property string value_type: "silver"
    property alias mouseEnabled: maItem.enabled
    property bool prefix_image_visible: false
    property alias prefix_image: btnPrefixImage.source
    property alias prefix_text: btnPrefixText.text
    property alias prefix_color: btnPrefixText.color

    signal clicked

    color: transparent ? "transparent" : "black"
    z: 100

    LargeText {
        id: btnPrefixText
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "white"
        text: ""
        height: 0.9 * parent.height
        width: 0
        visible: false
    }

    Image {
        id: btnPrefixImage
        fillMode: Image.Stretch
        source: "qrc:/res/level/BG_bottom.png";
        anchors.left: btnPrefixText.right
        anchors.verticalCenter:  parent.verticalCenter
        height: 0.9 * parent.height
        width: 0
        visible: false
    }

    LargeText {
        id: btnText
        anchors.left: btnPrefixImage.right
        anchors.right: btnIcon.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "white"
        text: "0"
    }

    Image {
        id: btnIcon
        fillMode: Image.Stretch
        source: "qrc:/res/level/"+ value_type + ".png";
        anchors.right: parent.right
        anchors.verticalCenter:  parent.verticalCenter
        height: 0.9 * parent.height
        width: 0.9 * parent.height
    }

    onValueChanged: {
        console.log("BtnWithIcon.onValueChanged", value_type, value);
        /*if (value_type === "rank") {
            btnIcon.source = "qrc:/res/level/rank_" + value + ".png";
            return;
        }*/
        btnText.text = TU.printNumber(value);
    }

    //Mouse area to react on click events
    MouseArea {
        id: maItem
        anchors.fill: button
        onClicked: {
            button.clicked();
        }
    }

    function onFinishCreation() {
        console.log("BtnWithIcon.onFinishCreation", value_type, value, prefix_image_visible, btnPrefixImage.source, prefix_text, btnPrefixImage.visible, btnPrefixImage.width);
        if (prefix_image_visible) {
            btnPrefixImage.width = btnIcon.width;
            btnPrefixImage.visible = true;
            btnPrefixText.visible = false;
            btnPrefixText.width = 0;
            //console.log("BtnWithIcon.onFinishCreation", "poiint 1");
        } else {
            btnPrefixImage.visible = false;
            btnPrefixImage.width = 0;
            //console.log("BtnWithIcon.onFinishCreation", "poiint 2");
        }

        if (prefix_text !== "") {
            btnPrefixText.width = 0.4 * button.width;
            btnPrefixText.visible = true;
            btnPrefixImage.visible = false;
            btnPrefixImage.width = 0;
            //console.log("BtnWithIcon.onFinishCreation", "poiint 3");
        } else {
            btnPrefixText.visible = false;
            btnPrefixText.width = 0;
            // console.log("BtnWithIcon.onFinishCreation", "poiint 4");
        }
        console.log("BtnWithIcon.onFinishCreation", btnPrefixImage.visible, btnPrefixImage.width);

        /*if (value_type === "rank") {
            btnIcon.source = "qrc:/res/level/rank_" + value + ".png";
            btnIcon.width = button.width;
            btnText.text = "";
        } else {
            btnIcon.source = "qrc:/res/level/"+ value_type + ".png";
        }*/
    }

    Component.onCompleted: onFinishCreation();
}
