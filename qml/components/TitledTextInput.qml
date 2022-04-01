import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: lbiMain

    //property bool active: true

    signal inputChanged

    property bool transparent: true
    property bool isPassword: false
    property alias text_color: tiMain.color
    property alias text: tiMain.text
    property alias fontSizeMode: ltiTitle.fontSizeMode
    property alias fontTitle: ltiTitle.font
    property alias fontEdit: tiMain.font
    property alias regularExpression: revValidator.regularExpression
    property alias title: ltiTitle.text
    property alias title_color: ltiTitle.color
    property alias placeholderText: tPlaceHolder.text
    property alias placeholderTextColor: tPlaceHolder.color
    property alias horizontalAlignment: ltiTitle.horizontalAlignment
    property alias verticalAlignment: ltiTitle.verticalAlignment
    property string bg_color: "snow"
    property string border_color: "black"
    property alias validator: tiMain.validator
    property int marginPrcLeft: 5
    property int marginPrcTop: 5

    LargeText {
        id: ltiTitle
        anchors.top: parent.top
        anchors.topMargin: marginPrcTop / 100 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: marginPrcLeft / 100 * parent.width
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
        anchors.topMargin: marginPrcTop / 100 * parent.height
        anchors.left: parent.left
        anchors.leftMargin: marginPrcLeft / 100 * parent.width
        radius: 5
        color: transparent ? "transparent" : bg_color
        border.color: border_color
        z: 10

        //TextField {
        TextInput {
            id: tiMain
            anchors.top: parent.top
            anchors.topMargin: 0.3 * parent.height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.2 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            width: 0.95 * parent.width
            inputMethodHints: Qt.ImhNoPredictiveText
            validator: RegularExpressionValidator { id: revValidator; regularExpression: /^[a-zA-Z0-9_-]{2,10}$/ }
            color: "black"
            echoMode: isPassword ? TextInput.Password : TextInput.Normal
            font.pixelSize: 0.3 * parent.height
            z: 11

            onTextChanged: {
                lbiMain.inputChanged();
            }

            Text {
                id: tPlaceHolder
                //anchors.fill: parent
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: -0.20 * parent.height
                height: 0.8 * parent.height
                visible: tiMain.text.length === 0
                font.pixelSize: tiMain.font.pixelSize
                z:10
            }
        }
    }

    function setFontSize(real_height) {
        console.log("LabeledTextInput.setFontSize", lbiMain.height, tiMain.height, real_height, tiMain.font.pixelSize);
        var h = real_height ? real_height : lbiMain.height;
        tiMain.font.pixelSize = 0.3 * h;
        console.log("LabeledTextInput.setFontSize after", tiMain.font.pixelSize);
    }

    function onFinishCreation() {
        //console.log("LabeledTextInput.onFinishCreation", lbiMain.height, tiMain.height, tiMain.font.pixelSize);
        //tiMain.font.pixelSize = 0.36 * lbiMain.height;
    }

    Component.onCompleted: onFinishCreation();
}
