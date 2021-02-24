import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: lbiMain

    //property bool active: true

    property bool transparent: true
    property bool isPassword: false
    property alias text_color: tiMain.color
    property alias text: tiMain.text
    property alias fontSizeMode: ltiTitle.fontSizeMode
    property alias regularExpression: revValidator.regularExpression
    property alias title: ltiTitle.text
    property alias title_color: ltiTitle.color
    property alias placeholderText: tiMain.placeholderText
    property alias placeholderTextColor: tiMain.placeholderTextColor
    property alias horizontalAlignment: ltiTitle.horizontalAlignment
    property alias verticalAlignment: ltiTitle.verticalAlignment
    property string bg_color: "snow"
    property string border_color: "black"
    property alias validator: tiMain.validator

    LargeText {
        id: ltiTitle
        anchors.top: parent.top
        anchors.topMargin: 0.05 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0.05 * parent.width
        height: 0.45 * parent.height
        width: 0.9 * parent.width
        color: "black"
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignLeft
    }

    Rectangle {
        id: rMain
        height: ltiTitle.height
        width: 0.9 * parent.width
        anchors.top: ltiTitle.bottom
        anchors.topMargin: 0.05 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0.05 * parent.width
        radius: 5
        color: transparent ? "transparent" : bg_color
        border.color: border_color

        TextField {
            id: tiMain
            anchors.fill: parent
            //height: 0.95 * parent.height
            //width: 0.95 * parent.width
            inputMethodHints: Qt.ImhNoPredictiveText
            validator: RegularExpressionValidator { id: revValidator; regularExpression: /^[a-zA-Z0-9_-]{2,10}$/ }
            color: "black"
            echoMode: isPassword ? TextInput.Password : TextInput.Normal
            font.pixelSize: 0.4 * parent.height
        }
    }

    function setFontSize(real_height) {
        console.log("LabeledTextInput.setFontSize", lbiMain.height, tiMain.height, real_height, tiMain.font.pixelSize);
        var h = real_height ? real_height : lbiMain.height;
        tiMain.font.pixelSize = 0.36 * h;
        console.log("LabeledTextInput.setFontSize after", tiMain.font.pixelSize);
    }

    function onFinishCreation() {
        //console.log("LabeledTextInput.onFinishCreation", lbiMain.height, tiMain.height, tiMain.font.pixelSize);
        //tiMain.font.pixelSize = 0.36 * lbiMain.height;
    }

    Component.onCompleted: onFinishCreation();
}
