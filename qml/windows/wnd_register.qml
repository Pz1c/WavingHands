// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: false
    body_width_prc: 100
    body_height_prc: 100
    title_height_prc: 0
    control_height_prc: 0
    bg_visible: false

    function changeIndex(diff) {
        var curr_idx = svWelcome.currentIndex;
        var n_diff = diff > 0 ? 1 : -1;
        var new_idx = curr_idx + n_diff;
        if (new_idx <= 0) {
            new_idx = svWelcome.count - 1;
        }
        if (new_idx >= svWelcome.count) {
            new_idx = 0;
        }
        console.log("changeIndex", diff, n_diff, curr_idx, new_idx);
        svWelcome.setCurrentIndex(new_idx);
    }


    // This rectangle is the actual popup
    Rectangle {
        id: mainItem
        anchors.fill: content_item
        z: 11
        color: "black"

        Image {
            id: iTopBg
            height: 0.25 * parent.height
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            source: "qrc:/res/bottom_glow.png"
            z: 12
        }

        BtnWithIcon {
            id: bwiLeft
            z: 15
            anchors.left: parent.left
            anchors.top: parent.top
            width: 0.26 * parent.width
            height: 0.04 * parent.height
            visible: svWelcome.currentIndex > 0

            image: "qrc:/res/back_arrow.png"
            image_anchors.verticalCenter: bwiLeft.verticalCenter
            image_anchors.left: bwiLeft.left
            icon_width: 0.08 * parent.width
            text_width: 0.1 * parent.width
            text_color: "#FEE2D6"
            text: "Back"



            onClicked: {
                changeIndex(-1);
            }
        }

        BtnWithIcon {
            id: bwiRight
            z: 15
            anchors.right: parent.right
            anchors.top: parent.top
            width: 0.26 * parent.width
            height: 0.04 * parent.height
            visible: svWelcome.currentIndex < 3//svWelcome.count - 1

            image: "qrc:/res/ahead_arrow.png"
            image_anchors.verticalCenter: bwiRight.verticalCenter
            image_anchors.right: bwiRight.right
            icon_width: 0.08 * parent.width
            text_width: 0.1 * parent.width
            text: "Next"
            text_color: "#FEE2D6"

            onClicked: {
                changeIndex(1);
            }
        }

        SwipeView {
            id: svWelcome
            anchors.fill: parent
            currentIndex: 0
            z: 13

            onCurrentIndexChanged: {
                var new_idx = svWelcome.currentIndex;
                console.log("svWelcome.onCurrentIndexChanged", new_idx);
                bwiRight.visible = new_idx < 3;
                bwiLeft.visible = bwiRight.visible && (new_idx > 0);
            }

            Item {
                id: svScreen01

                Image {
                    id: svs1Title
                    source: "qrc:/res/reg_ttl_01.png"
                    width: 0.8 * parent.width
                    height: 0.5 * width
                    anchors.top: parent.top
                    anchors.topMargin: 0.1 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: svs1Bottom
                    source: "qrc:/res/reg_img_01.png"
                    width: 0.8 * parent.width
                    height: 1.4 * width
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Item {
                id: svScreen02

                Image {
                    id: svs2Title
                    source: "qrc:/res/reg_ttl_02.png"
                    width: 0.8 * parent.width
                    height: 0.5 * width
                    anchors.top: parent.top
                    anchors.topMargin: 0.1 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: svs2Bottom
                    source: "qrc:/res/reg_img_02.png"
                    width: 0.8 * parent.width
                    height: 1.4 * width
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Item {
                id: svScreen03

                Image {
                    id: svs3Title
                    source: "qrc:/res/reg_ttl_03.png"
                    width: 0.8 * parent.width
                    height: 0.5 * width
                    anchors.top: parent.top
                    anchors.topMargin: 0.1 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: svs3Bottom
                    source: "qrc:/res/reg_img_03.png"
                    width: 0.8 * parent.width
                    height: 1.4 * width
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Item {
                id: svScreenSingUp

                Image {
                    id: svsSingUpTitle
                    source: "qrc:/res/reg_ttl_04.png"
                    width: 0.5 * parent.width
                    height: 0.39 * width
                    anchors.top: parent.top
                    anchors.topMargin: 0.1 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                LargeText {
                    id: svsSingUpLink
                    color: "#E7FFFF"
                    text: "Already have an account? <u><font color=\"#A8F4F4\">Log In</font></u>"

                    //textFormat: Text.RichText
                    height: 0.05 * dMainItem.height
                    width: 0.6 * dMainItem.width
                    //bg_visible: true;
                    //bg_color: "white"
                    //fontSizeMode: Text.Fit
                    anchors.top: svsSingUpTitle.bottom
                    //anchors.topMargin: 0.01 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        //console.log("svsSingUpLink", width, height, dMainItem.height, dMainItem.width);
                        changeIndex(1);
                    }
                }

                LabeledTextInput {
                    id: ltiLogin
                    anchors.top: svsSingUpLink.bottom
                    //anchors.topMargin: 0.02 * parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 0.09 * parent.height
                    width: parent.width
                    title: "Warlock's name"//dict.getStringByCode("WarlockName")
                    title_color: "#E7FFFF"
                    border_color: "#E7FFFF"
                    text_color: "#E7FFFF"
                    placeholderText: ""
                    regularExpression: /^[a-zA-Z0-9_-]{2,10}$/
                }

                LabeledTextInput {
                    id: ltiEmail
                    anchors.top: ltiLogin.bottom
                    //anchors.topMargin: 0.02 * parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 0.09 * parent.height
                    width: parent.width
                    title: dict.getStringByCode("Email")
                    title_color: "#E7FFFF"
                    border_color: "#E7FFFF"
                    text_color: "#E7FFFF"
                    placeholderText: ""
                    regularExpression: /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/
                }

                LabeledTextInput {
                    id: ltiPass
                    anchors.top: ltiEmail.bottom
                    //anchors.topMargin: 0.02 * parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 0.09 * parent.height
                    width: parent.width
                    title: dict.getStringByCode("Password")
                    title_color: "#E7FFFF"
                    border_color: "#E7FFFF"
                    text_color: "#E7FFFF"
                    isPassword: true
                    placeholderText: ""
                    regularExpression: /^.{4,10}$/
                }

                RCheckBox {
                    id: cbConfirmation
                    anchors.left: parent.left
                    anchors.leftMargin: 0.05 * parent.width
                    anchors.verticalCenter: tConfirmation.verticalCenter
                    checked: false
                    height: 0.05 * parent.width
                    width: 0.05 * parent.width
                    border.color: "#E7FFFF"
                    border.width: 3
                    innerBorder.width: 2
                    fill_color_checked: "#E7FFFF"
                    fill_color_unchecked: "black"
                }

                LargeText {
                    id: tConfirmation
                    height: 0.07 * parent.height
                    width: 0.8 * parent.width
                    anchors.top: ltiPass.bottom
                    anchors.left: cbConfirmation.right
                    //anchors.topMargin: 0.02 * parent.height
                    //anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignLeft
                    text: dict.getStringByCode("Iam13")
                    color: "#E7FFFF"
                    onClicked: {
                        cbConfirmation.checked = !cbConfirmation.checked
                    }
                }

                BtnWithIcon {
                    id: bwiSingUp
                    z: 15
                    anchors.top: tConfirmation.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 0.05 * parent.width
                    width: 0.50 * parent.width
                    height: 0.09 * parent.height

                    image: cbConfirmation.checked ? "qrc:/res/send_1.png" : "qrc:/res/send_0.png"
                    //image_anchors.verticalCenter: bwiSingUp.verticalCenter
                    image_anchors.right: bwiSingUp.right
                    icon_width: 0.1 * parent.width
                    icon_height: 0.06 * parent.height
                    text_width: 0.2 * parent.width
                    text_height: 0.08 * parent.height

                    text: "Submit"
                    text_color: cbConfirmation.checked ? "#E7FFFF" : "#5E5C5E"

                    onClicked: {
                        var err_cnt = 0;
                        if ((ltiLogin.text.length < 2) || (ltiLogin.text.length > 10)) {
                            ltiLogin.border_color = "red";
                            ++err_cnt;
                        } else {
                            ltiLogin.border_color = "#E7FFFF";
                        }

                        if ((ltiEmail.text.indexOf("@") === -1) || (ltiEmail.text.indexOf(".") === -1)|| (ltiEmail.text.length < 6)) {
                            ltiEmail.border_color = "red";
                            ++err_cnt;
                        } else {
                            ltiEmail.border_color = "#E7FFFF";
                        }

                        if ((ltiPass.text.length < 4)|| (ltiPass.text.length > 10)) {
                            ltiPass.border_color = "red";
                            ++err_cnt;
                        } else {
                            ltiPass.border_color = "#E7FFFF";
                        }

                        if (!cbConfirmation.checked) {
                            cbConfirmation.border.color = "red"
                            tConfirmation.color = "red"
                            ++err_cnt;
                        } else {
                            cbConfirmation.border.color = "#E7FFFF";
                            tConfirmation.color = "#E7FFFF";
                        }

                        if (err_cnt === 0) {
                            mainWindow.gameCore.regNewUser(ltiLogin.text, ltiEmail.text, ltiPass.text);
                        }
                    }
                }

                Image {
                    id: svsSingUpBottom
                    source: "qrc:/res/reg_img_04.png"
                    width: 0.8 * parent.width
                    height: 0.61 * width
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Item {
                id: svScreenLogin

                Image {
                    id: svsLoginTitle
                    source: "qrc:/res/reg_ttl_05.png"
                    width: 0.5 * parent.width
                    height: 0.39 * width
                    anchors.top: parent.top
                    anchors.topMargin: 0.1 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                LargeText {
                    id: svsLoginLink
                    color: "#E7FFFF"
                    text: "Don't have an account? <u><font color=\"#A8F4F4\">Sign Up</font></u>"

                    //textFormat: Text.RichText
                    height: 0.05 * dMainItem.height
                    width: 0.6 * dMainItem.width
                    //bg_visible: true;
                    //bg_color: "white"
                    //fontSizeMode: Text.Fit
                    anchors.top: svsLoginTitle.bottom
                    //anchors.topMargin: 0.01 * mainItem.height
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        //console.log("svsSingUpLink", width, height, dMainItem.height, dMainItem.width);
                        changeIndex(-1);
                    }
                }

                LabeledTextInput {
                    id: ltiLLogin
                    anchors.top: svsLoginLink.bottom
                    //anchors.topMargin: 0.02 * parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 0.09 * parent.height
                    width: parent.width
                    title: "Warlock's name"//dict.getStringByCode("WarlockName")
                    title_color: "#E7FFFF"
                    border_color: "#E7FFFF"
                    text_color: "#E7FFFF"
                    placeholderText: ""
                    regularExpression: /^[a-zA-Z0-9_-]{2,10}$/
                }

                LabeledTextInput {
                    id: ltiLPass
                    anchors.top: ltiLLogin.bottom
                    //anchors.topMargin: 0.02 * parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 0.09 * parent.height
                    width: parent.width
                    title: dict.getStringByCode("Password")
                    title_color: "#E7FFFF"
                    border_color: "#E7FFFF"
                    text_color: "#E7FFFF"
                    isPassword: true
                    placeholderText: ""
                    regularExpression: /^.{4,10}$/
                }

                BtnWithIcon {
                    id: bwiLogIn
                    z: 15
                    anchors.top: ltiLPass.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 0.05 * parent.width
                    width: 0.50 * parent.width
                    height: 0.09 * parent.height

                    image: "qrc:/res/send_1.png"
                    image_anchors.right: bwiLogIn.right
                    icon_width: 0.1 * parent.width
                    icon_height: 0.06 * parent.height
                    text_width: 0.2 * parent.width
                    text_height: 0.08 * parent.height

                    text: "Submit"
                    text_color: "#E7FFFF"

                    onClicked: {
                        var err_cnt = 0;
                        if ((ltiLLogin.text.length < 2) || (ltiLLogin.text.length > 10)) {
                            ltiLLogin.border_color = "red";
                            ++err_cnt;
                        } else {
                            ltiLLogin.border_color = "#E7FFFF";
                        }

                        if ((ltiLPass.text.length < 4)|| (ltiLPass.text.length > 10)) {
                            ltiLPass.border_color = "red";
                            ++err_cnt;
                        } else {
                            ltiLPass.border_color = "#E7FFFF";
                        }

                        if (err_cnt === 0) {
                            mainWindow.gameCore.setLogin(ltiLLogin.text, ltiLPass.text);
                        }
                    }
                }

                Image {
                    id: svsLoginBottom
                    source: "qrc:/res/reg_img_04.png"
                    width: 0.8 * parent.width
                    height: 0.61 * width
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        PageIndicator {
            id: indicator

            count: svWelcome.count
            currentIndex: svWelcome.currentIndex

            anchors.bottom: svWelcome.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        console.log("wnd_register.onCompleted", svWelcome.z, content_item.z, svWelcome.height, ltiLLogin.height);
        ltiLogin.setFontSize();
        ltiEmail.setFontSize();
        ltiPass.setFontSize();
        ltiLLogin.setFontSize();
        ltiLPass.setFontSize();
    }
}
