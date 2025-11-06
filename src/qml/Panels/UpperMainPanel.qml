import QtQuick

import Components
import Bee as Backend

Circle {
    id: upperMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.
    property Item lowerMenuOverlay

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: setupPanel.enabled ? setupPanel : clockPanel

        currentIndex: indexOfPanel(initialPanel)

        SetupPanel {
            id: setupPanel

            anchors.fill: parent
            enabled: !Backend.Applications.setup.setupComplete
            lowerMenuOverlay: upperMainPanel.lowerMenuOverlay

            onFinished: {
                panelContainer.showPanel(clockPanel)
            }

            onEnabledChanged: {
                if (enabled) {
                    panelContainer.showPanel(setupPanel)
                    menuOverlay.visible = false
                }
            }
        }

        ClockPanel {
            id: clockPanel

            anchors.fill: parent
            enabled: Backend.Applications.clock.configuration.enabled

            onClicked: menuOverlay.visible = true
        }

        SevenSegmentPanel {
            id: marriedPanel

            anchors.fill: parent
            enabled: Backend.Applications.marriedTimer.configuration.enabled
            initialized: Backend.Applications.marriedTimer.configuration.initialized
            onClicked: menuOverlay.visible = true
            years: Backend.Applications.marriedTimer.years
            days: Backend.Applications.marriedTimer.days
            hours: Backend.Applications.marriedTimer.hours
            minutes: Backend.Applications.marriedTimer.minutes
            seconds: Backend.Applications.marriedTimer.seconds
        }

        RoundProgressBarPanel {
            id: kuikenBirthdayPanel

            anchors.fill: parent
            enabled: Backend.Applications.kuikenTimer.configuration.enabled
            initialized: Backend.Applications.kuikenTimer.configuration.initialized
            onClicked: menuOverlay.visible = true
            years: Backend.Applications.kuikenTimer.years
            days: Backend.Applications.kuikenTimer.days
            daysInWeek: Backend.Applications.kuikenTimer.daysInWeek
            weeks: Backend.Applications.kuikenTimer.weeks
            hours: Backend.Applications.kuikenTimer.hours
            minutes: Backend.Applications.kuikenTimer.minutes
            seconds: Backend.Applications.kuikenTimer.seconds
            barColor: Backend.Applications.kuikenTimer.configuration.baseColor
            textColor: Backend.Applications.kuikenTimer.configuration.accentColor
            backgroundSource: Backend.Services.media.getMediaPath(Backend.Applications.kuikenTimer.configuration.background)
            backgroundOpacity: Backend.Applications.kuikenTimer.configuration.backgroundOpacity
        }

        CountdownPanel {
            id: countdownTimer

            anchors.fill: parent
            backgroundSource: Backend.Services.media.getMediaPath(Backend.Applications.countdownTimer.configuration.background)
            backgroundOpacity: Backend.Applications.countdownTimer.configuration.backgroundOpacity
            enabled: Backend.Applications.countdownTimer.configuration.enabled
            initialized: Backend.Applications.countdownTimer.configuration.initialized
            days: Backend.Applications.countdownTimer.days
            hours: Backend.Applications.countdownTimer.hours
            minutes: Backend.Applications.countdownTimer.minutes
            seconds: Backend.Applications.countdownTimer.seconds
            onClicked: menuOverlay.visible = true
        }
    }

    UpperMenuOverlay {
        id: menuOverlay

        visible: false
        anchors.fill: parent

        onClose: {
            menuOverlay.visible = false
            Backend.Applications.menu.closeDialog()
        }
    }

    Timer {
        id: panelRotationTimer

        interval: 10000 // 10 seconds
        running: panelContainer.currentIndex != panelContainer.indexOfPanel(setupPanel)
        repeat: true

        property bool direction: true
        onTriggered: {
            if (!panelContainer.nextPanel()) {
                panelContainer.showPanel(panelContainer.initialPanel);
            }
        }
    }

    NotificationOverlay {
        id: notificationOverlay
        anchors.fill: parent

        z: 1000 // Ensure it appears above other content
    }
}
