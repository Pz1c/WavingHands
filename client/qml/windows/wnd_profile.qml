// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"
import "qrc:/js/user_profile_utils.js" as UPUtils

BaseWindow {
    id: dMainItem
    with_controls: true
    with_apply: false
    title_text: dict.getStringByCode("EnterNewUser")
    //body_height_prc: 80

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.fill: content_item
        color: "snow"
        z: 11

        ScrollView {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: rSendMsg.top

            width: dialogWindow.width
            height: dialogWindow.height - btnDoit.height

            Text {
                  x: 5
                  y: 0
                  id: tPlayerInfo
                  width: dialogWindow.width - 30
                  //height: 200
                  //anchors.fill: parent
                  text: ""
                  anchors.topMargin: 8
                  //font.pointSize: 13 * height_koeff
                  wrapMode: Text.WordWrap
                  textFormat: Text.RichText
                  onLinkActivated: UPUtils.linkActivated(link)
            }
        }

        Rectangle {
            id: rSendMsg
            height: 0.2 * parent.height
            width: parent.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: btnDoit.top
            border.width: 1
            border.color: "grey"
            radius: 3

            Rectangle {
                id: rTxtMsg
                anchors.left: parent.left
                anchors.right: btnSendTxt.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                radius: 7
                border.width: 1
                border.color: "black"

                TextInput {
                    id: teTxtMsg
                    width: parent.width - 6
                    height: parent.height - 2
                    anchors.centerIn: parent
                    text: ""
                    ////font.pointSize: 12 * height_koeff
                    inputMethodHints: Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
                    //echoMode: TextInput.Password
                }
            }

            Rectangle {
                id: btnSendTxt
                anchors.right: parent.right
                anchors.top: parent.top
                width: 0.25 * parent.width
                height: parent.height
                color: maSendMsg.pressed ? "green" : "lightgreen"
                radius: 7
                anchors.horizontalCenterOffset: 0
                border.width: 3
                border.color: "green"

                Text {
                    id: labelSendMsg
                    anchors.centerIn: parent
                    text: warlockDictionary.getStringByCode("Submit")
                    ////font.pointSize: 16 * height_koeff
                }

                MouseArea {
                    id: maSendMsg
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        console.log("try core.sendMessage", teTxtMsg.text, teTxtMsg.text.length);
                        if (teTxtMsg.text.length > 0) {
                            Qt.core.sendMessage(teTxtMsg.text);
                        }
                        //MUtils.sendOrder()
                    }
                }
            }
        }
    }

    onCancel: {
        Qt.gameField.processEscape();
    }

    function initProfileFields() {
        console.log("user_profile.initProfileFields", Qt.show_info_self);
        tPlayerInfo.text = Qt.show_info_self ? Qt.core.playerInfo : Qt.core.warlockInfo;
        rSendMsg.visible = !Qt.show_info_self;
    }

    Component.onCompleted: initProfileFields()
}
