import QtQuick

import Bee as BeeBackend

Circle {
    id: notificationOverlay

    color: Qt.rgba(0, 0, 0, 0.7)

    visible: BeeBackend.Services.notification.isVisible && BeeBackend.Services.notification.model.activeCount > 0

    // Make it clickable to inactivate notifications
    MouseArea {
        anchors.fill: parent
        onClicked: {
            // Inactivate the highest priority active notification (the one currently showing)
            let highestNotification = BeeBackend.Services.notification.model.highestPriorityNotification
            if (highestNotification && highestNotification.isActive) {
                BeeBackend.Services.notification.model.setNotificationActive(highestNotification.id, false)
            }
        }
    }

    // Title and notification content
    Column {
        anchors.centerIn: parent
        width: parent.width - 40
        spacing: 10

        // Notification title
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: BeeBackend.Services.notification.model.highestPriorityNotification ? BeeBackend.Services.notification.model.highestPriorityNotification.title : ""
            color: "white"
            font.pixelSize: 62
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        // Show only the highest priority active notification
        Text {
            width: parent.width
            text: BeeBackend.Services.notification.model.highestPriorityNotification ? BeeBackend.Services.notification.model.highestPriorityNotification.message : ""
            color: "white"
            font.pixelSize: 18
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
