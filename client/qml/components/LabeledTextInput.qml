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
    property string bg_color: "white"
    property string border_color: "white"

    LargeText {
        id: ltiTitle
        anchors.top: parent.top
        anchors.topMargin: 0.05 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0.05 * parent.width
        height: 0.45 * parent.height
        width: 0.9 * dialogWindow.width
        color: "yellow"
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignLeft
    }

    TextField {
        id: tiMain
        height: ltiTitle.height
        width: 0.9 * dialogWindow.width
        anchors.top: ltiTitle.bottom
        anchors.topMargin: 0.05 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0.05 * parent.width
        inputMethodHints: Qt.ImhNoPredictiveText
        validator: RegularExpressionValidator { id: revValidator; regularExpression: /^[a-zA-Z0-9_-]{2,10}$/ }
        color: "white"
        echoMode: isPassword ? TextInput.Password : TextInput.Normal
        font.pixelSize: 0.8 * height
        background: Rectangle {
            radius: 5
            //implicitWidth: 100
            //implicitHeight: 24
            color: transparent ? "transparent" : bg_color
            border.color: border_color
        }
    }

    function onFinishCreation() {
        console.log("LabeledTextInput.onFinishCreation");
    }

    Component.onCompleted: onFinishCreation();
}
