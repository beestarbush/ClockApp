import QtQuick
import Components
import Bee as BeeBackend

Item {
    Column {
        spacing: 10
        padding: 20

        Button {
            text: "Test Info"
            onClicked: BeeBackend.Services.notificationManager.showInfo("Information", "This is an info notification")
        }

        Button {
            text: "Test Warning"
            onClicked: BeeBackend.Services.notificationManager.showWarning("Warning", "This is a warning notification")
        }

        Button {
            text: "Test Error"
            onClicked: BeeBackend.Services.notificationManager.showError("Error", "This is an error notification")
        }

        Button {
            text: "Clear All"
            onClicked: BeeBackend.Services.notificationManager.clearAll()
        }

        Button {
            text: "Clear Inactive"
            onClicked: BeeBackend.Services.notificationManager.clearInactive()
        }

        Button {
            text: "Inactivate Top"
            onClicked: {
                let highest = BeeBackend.Services.notificationManager.highestPriorityNotification
                if (highest && highest.isActive) {
                    BeeBackend.Services.notificationManager.setNotificationActive(highest.id, false)
                }
            }
        }

        Text {
            text: "Total Count: " + BeeBackend.Services.notificationManager.count
            color: "white"
        }

        Text {
            text: "Active Count: " + BeeBackend.Services.notificationManager.activeCount
            color: "white"
        }

        Text {
            text: "Has Active: " + BeeBackend.Services.notificationManager.hasNotifications
            color: "white"
        }

        Text {
            text: "Is Visible: " + BeeBackend.Services.notificationManager.isVisible
            color: "white"
        }
    }
}