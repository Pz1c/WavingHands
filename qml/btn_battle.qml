import QtQuick 2.0

Rectangle {
    id: btnBattle
    property int battle_id: 0
    property int battle_type: 0 // 0 ready, 1 wating, 2 finished
    width: (battle_id + ' ').length * 5
    height: 16
    //border.width: 1
    //border.color: "blue"

    Text {
        id: btnText
        anchors.centerIn: parent
        text: battle_id
        font.underline: true
        color: "blue"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.core.getBattle(battle_id, battle_type);
            btnBattle.parent.parent.parent.destroy()
        }
    }
}
