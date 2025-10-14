import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

Item {
    id: notificationCarousel

    property var notifications: []
    property int currentIndex: 0
    property var selectedNotification: notifications.length > 0 ? notifications[currentIndex] : null

    signal notificationSelected(var notification)

    function setNotifications(notificationList) {
        notifications = notificationList || []
        if (currentIndex >= notifications.length) {
            currentIndex = Math.max(0, notifications.length - 1)
        }
    }

    function selectCurrentNotification() {
        if (selectedNotification) {
            notificationSelected(selectedNotification)
        }
    }

    function toggleCurrentNotification() {
        if (selectedNotification) {
            BeeBackend.Services.notificationManager.toggleNotificationActive(selectedNotification.notificationId)
        }
    }

    // No notifications message
    Text {
        id: noNotificationsText
        anchors.centerIn: parent
        text: "No notifications"
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        horizontalAlignment: Text.AlignHCenter
        visible: notifications.length === 0
    }

    // Main flickable for swiping through notifications
    Flickable {
        id: flickable
        anchors.fill: parent
        
        contentWidth: notificationRow.width
        contentHeight: height
        flickDeceleration: 1500
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        interactive: notifications.length > 1
        
        visible: notifications.length > 0

        Row {
            id: notificationRow
            height: parent.height
            spacing: 0

            Repeater {
                model: notifications

                delegate: Item {
                    id: notificationItem
                    width: flickable.width
                    height: flickable.height

                    property bool isSelected: index === currentIndex
                    property var notification: modelData

                    // Full screen notification preview
                    Circle {
                        id: notificationCircle
                        anchors.fill: parent
                        color: notification.isActive ? Qt.rgba(0, 0, 0, 0.8) : Qt.rgba(0.5, 0.5, 0.5, 0.6)
                        border.color: notification.isActive ? Color.lightGray : Color.gray
                        border.width: 2
                        opacity: isSelected ? 1.0 : 0.3

                        Behavior on opacity {
                            NumberAnimation { duration: 300 }
                        }

                        // Notification content
                        Column {
                            anchors.centerIn: parent
                            width: parent.width - Value.defaultMargin * 4
                            spacing: Value.defaultMargin

                            // Status indicator
                            Rectangle {
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: Value.defaultMargin
                                height: width
                                radius: width / 2
                                color: notification.isActive ? Color.green1 : Color.gray
                            }

                            // Notification message
                            Text {
                                id: messageText
                                width: parent.width
                                text: notification.message
                                color: notification.isActive ? Color.lightGray : Color.gray
                                font.pixelSize: Value.defaultTextSize
                                font.bold: isSelected
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                            }

                            // Notification ID (for debugging/reference)
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "ID: " + notification.notificationId
                                color: notification.isActive ? Color.lightGray : Color.gray
                                font.pixelSize: Value.defaultTextSize * 0.8
                                opacity: 0.6
                                visible: isSelected
                            }

                            // Status text
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: notification.isActive ? "Active" : "Inactive"
                                color: notification.isActive ? Color.green1 : Color.gray
                                font.pixelSize: Value.defaultTextSize
                                font.bold: true
                                visible: isSelected
                            }
                        }
                    }

                    // Click to select/toggle
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (currentIndex === index) {
                                // If already selected, toggle the notification
                                toggleCurrentNotification()
                            } else {
                                // Select this notification
                                currentIndex = index
                                selectCurrentNotification()
                            }
                        }
                    }
                }
            }
        }

        // Snap to pages when flicking stops
        onMovementEnded: {
            var targetIndex = Math.round(contentX / width)
            targetIndex = Math.max(0, Math.min(targetIndex, notifications.length - 1))
            
            if (targetIndex !== currentIndex) {
                currentIndex = targetIndex
            }
            
            // Smooth scroll to the selected page
            snapAnimation.to = targetIndex * width
            snapAnimation.start()
        }

        NumberAnimation {
            id: snapAnimation
            target: flickable
            property: "contentX"
            duration: 300
            easing.type: Easing.OutCubic
        }
    }

    // Page indicators at bottom
    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: Value.smallMargin
        spacing: Value.smallMargin
        visible: notifications.length > 1

        Repeater {
            model: notifications.length

            delegate: Rectangle {
                width: 15
                height: width
                radius: width / 2
                color: {
                    if (index === currentIndex) {
                        return notifications[index] && notifications[index].isActive ? Color.green1 : Color.lightGray
                    }
                    return Color.gray
                }
                opacity: index === currentIndex ? 1.0 : 0.5

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }
            }
        }
    }

    // Navigation arrows for additional clarity
    Rectangle {
        id: leftArrow
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: Value.defaultMargin
        width: Value.defaultMargin * 2
        height: width
        radius: width / 2
        color: Color.black
        opacity: currentIndex > 0 ? 0.7 : 0.3
        visible: notifications.length > 1

        Text {
            anchors.centerIn: parent
            text: "‹"
            color: Color.lightGray
            font.pixelSize: Value.largeTextSize
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: currentIndex > 0
            onClicked: {
                currentIndex = Math.max(0, currentIndex - 1)
                snapAnimation.to = currentIndex * flickable.width
                snapAnimation.start()
            }
        }
    }

    Rectangle {
        id: rightArrow
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: Value.defaultMargin
        width: Value.defaultMargin * 2
        height: width
        radius: width / 2
        color: Color.black
        opacity: currentIndex < notifications.length - 1 ? 0.7 : 0.3
        visible: notifications.length > 1

        Text {
            anchors.centerIn: parent
            text: "›"
            color: Color.lightGray
            font.pixelSize: Value.largeTextSize
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: currentIndex < notifications.length - 1
            onClicked: {
                currentIndex = Math.min(notifications.length - 1, currentIndex + 1)
                snapAnimation.to = currentIndex * flickable.width
                snapAnimation.start()
            }
        }
    }

    // Update notifications when notification manager changes
    Connections {
        target: BeeBackend.Services.notificationManager
        function onCountChanged() {
            updateNotifications()
        }
    }

    function updateNotifications() {
        var notificationList = []
        var notificationManager = BeeBackend.Services.notificationManager
        
        // Convert model to array (assuming the model has a way to iterate)
        for (var i = 0; i < notificationManager.count; i++) {
            var notification = notificationManager.get(i)
            if (notification) {
                notificationList.push(notification)
            }
        }
        
        setNotifications(notificationList)
    }

    Component.onCompleted: {
        updateNotifications()
    }
}
