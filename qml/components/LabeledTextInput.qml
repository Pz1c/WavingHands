import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: lbiMain

    //property bool active: true

    signal inputChanged

    property bool transparent: true
    property bool isPassword: false
    property bool hidePassword: true
    property alias text_color: tiMain.color
    property string text: ""
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
            //echoMode: (isPassword && hidePassword) ? TextInput.Password : TextInput.Normal
            echoMode: TextInput.Normal
            font.pixelSize: 28 * mainWindow.ratioFont
            z: 11

            onTextEdited: {
                console.log("onTextChanged.1", tiMain.text, tiMain.text.length, lbiMain.text, lbiMain.text.length, tiMain.text.indexOf("*"));
                if (isPassword && hidePassword) {
                    if (tiMain.text.indexOf("*") === 0) {
                        if (tiMain.text.length > lbiMain.text.length) {
                            //console.log("onTextChanged.2", lbiMain.text.length, tiMain.text.length - lbiMain.text.length, "!" + tiMain.text.substring(lbiMain.text.length - 1, tiMain.text.length - lbiMain.text.length) + "!");

                            lbiMain.text += tiMain.text.substring(lbiMain.text.length);
                        } else {
                            lbiMain.text = lbiMain.text.substring(0, tiMain.text.length);
                        }
                    } else {
                        lbiMain.text = tiMain.text;
                    }

                    tiMain.text = generatePasswordMask(lbiMain.text.length);
                } else {
                    lbiMain.text = tiMain.text;
                }//*/
                lbiMain.inputChanged();
            }

//            onEchoModeChanged: {
//                font.pixelSize = 28 * mainWindow.ratioFont;
//            }

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

            Image {
                id: iPassEye
                anchors.right: parent.right
                anchors.rightMargin: 0.03 * parent.height
                anchors.verticalCenter: parent.verticalCenter
                height: 0.9 * parent.height
                width: parent.height
                source: hidePassword ? "qrc:/res/no-eye.png" : "qrc:/res/eye.png"
                visible: isPassword

                MouseArea {
                    id: maPassEye
                    anchors.fill: parent
                    onClicked: {
                        hidePassword = !hidePassword;
                        if (hidePassword) {
                            tiMain.text = generatePasswordMask(lbiMain.text.length);
                        } else {
                            tiMain.text = lbiMain.text;
                        }
                    }
                }
            }
        }
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function generatePasswordMask(length) {
        var res = "";
        for(var i = 0; i < length; ++i) {
            res += "*";
        }
        return res;
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
