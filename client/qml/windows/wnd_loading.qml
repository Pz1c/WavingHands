import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: dialogLoading
    anchors.fill: parent

    // Add a simple animation to fade in the popup
    // let the opacity go from 0 to 1 in 400ms
    PropertyAnimation {
        target: dialogLoading
        property: "opacity"
        duration: 400
        from: 0
        to: 1
        easing.type: Easing.InOutQuad
        running: true
    }

    // This rectange is the a overlay to partially show the parent through it
    // and clicking outside of the 'dialog' popup will do 'nothing'
    Rectangle {
        anchors.fill: parent
        id: overlay
        color: "#000000"
        opacity: 0.6
        // add a mouse area so that clicks outside
        // the dialog window will not do anything
        MouseArea {
            anchors.fill: parent
        }
    }

    BusyIndicator {
        id: aiLoading
        anchors.centerIn: parent
        running: true
        width: 0.5 * parent.width
        height: 0.5 * parent.width
    }

    function showWnd() {
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }
}
