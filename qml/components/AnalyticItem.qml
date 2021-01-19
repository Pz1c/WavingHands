import QtQuick 2.9
import QtFirebase 1.0

Analytics {
    id: analytics
    enabled: true
    minimumSessionDuration: 1000
    sessionTimeout: 5000
    userProperties: [
        {"screen_size" : Qt.screenWidth + "x" + Qt.screenHeight},
        {"os" : Qt.platform.os}
    ]

    onReadyChanged: {}
}

