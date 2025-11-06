import QtQuick
import Components
import Bee as BeeBackend

Item {
    Column {
        spacing: 10
        padding: 20

        Button {
            text: "Test Info"
            onClicked: BeeBackend.Services.notification.showInfo("Information", "This is an info notification")
        }

        Button {
            text: "Test Warning"
            onClicked: BeeBackend.Services.notification.showWarning("Warning", "This is a warning notification")
        }

        Button {
            text: "Test Error"
            onClicked: BeeBackend.Services.notification.showError("Error", "This is an error notification")
        }

        Button {
            text: "Clear All"
            onClicked: BeeBackend.Services.notification.model.clearAll()
        }

        Button {
            text: "Clear Inactive"
            onClicked: BeeBackend.Services.notification.model.clearInactive()
        }

        Button {
            text: "Inactivate Top"
            onClicked: {
                let highest = BeeBackend.Services.notification.model.highestPriorityNotification
                if (highest && highest.isActive) {
                    BeeBackend.Services.notification.model.setNotificationActive(highest.id, false)
                }
            }
        }

        Text {
            text: "Total Count: " + BeeBackend.Services.notification.model.count
            color: "white"
        }

        Text {
            text: "Active Count: " + BeeBackend.Services.notification.model.activeCount
            color: "white"
        }

        Text {
            text: "Has Active: " + BeeBackend.Services.notification.model.hasNotifications
            color: "white"
        }

        Text {
            text: "Is Visible: " + BeeBackend.Services.notification.isVisible
            color: "white"
        }
    }
}