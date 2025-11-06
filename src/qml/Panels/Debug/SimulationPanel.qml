import QtQuick
import Components
import Bee as Backend

Item {
    Column {
        spacing: 10
        padding: 20

        Button {
            text: "Test Info"
            onClicked: Backend.Services.notification.showInfo("Information", "This is an info notification")
        }

        Button {
            text: "Test Warning"
            onClicked: Backend.Services.notification.showWarning("Warning", "This is a warning notification")
        }

        Button {
            text: "Test Error"
            onClicked: Backend.Services.notification.showError("Error", "This is an error notification")
        }

        Button {
            text: "Clear All"
            onClicked: Backend.Services.notification.model.clearAll()
        }

        Button {
            text: "Clear Inactive"
            onClicked: Backend.Services.notification.model.clearInactive()
        }

        Button {
            text: "Inactivate Top"
            onClicked: {
                let highest = Backend.Services.notification.model.highestPriorityNotification
                if (highest && highest.isActive) {
                    Backend.Services.notification.model.setNotificationActive(highest.id, false)
                }
            }
        }

        Text {
            text: "Total Count: " + Backend.Services.notification.model.count
            color: "white"
        }

        Text {
            text: "Active Count: " + Backend.Services.notification.model.activeCount
            color: "white"
        }

        Text {
            text: "Has Active: " + Backend.Services.notification.model.hasNotifications
            color: "white"
        }

        Text {
            text: "Is Visible: " + Backend.Services.notification.isVisible
            color: "white"
        }
    }
}