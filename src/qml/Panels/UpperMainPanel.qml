import QtQuick

import Components
import Bee as BeeBackend

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
            enabled: !BeeBackend.Applications.setup.setupComplete
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
            enabled: BeeBackend.Applications.clock.configuration.enabled

            onClicked: menuOverlay.visible = true
        }

        SevenSegmentPanel {
            id: marriedPanel

            anchors.fill: parent
            enabled: BeeBackend.Applications.marriedTimer.configuration.enabled
            initialized: BeeBackend.Applications.marriedTimer.configuration.initialized
            onClicked: menuOverlay.visible = true
            years: BeeBackend.Applications.marriedTimer.years
            days: BeeBackend.Applications.marriedTimer.days
            hours: BeeBackend.Applications.marriedTimer.hours
            minutes: BeeBackend.Applications.marriedTimer.minutes
            seconds: BeeBackend.Applications.marriedTimer.seconds
        }

        RoundProgressBarPanel {
            id: kuikenBirthdayPanel

            anchors.fill: parent
            enabled: BeeBackend.Applications.kuikenTimer.configuration.enabled
            initialized: BeeBackend.Applications.kuikenTimer.configuration.initialized
            onClicked: menuOverlay.visible = true
            years: BeeBackend.Applications.kuikenTimer.years
            days: BeeBackend.Applications.kuikenTimer.days
            daysInWeek: BeeBackend.Applications.kuikenTimer.daysInWeek
            weeks: BeeBackend.Applications.kuikenTimer.weeks
            hours: BeeBackend.Applications.kuikenTimer.hours
            minutes: BeeBackend.Applications.kuikenTimer.minutes
            seconds: BeeBackend.Applications.kuikenTimer.seconds
            barColor: BeeBackend.Applications.kuikenTimer.configuration.baseColor
            textColor: BeeBackend.Applications.kuikenTimer.configuration.accentColor
            backgroundSource: BeeBackend.Services.mediaManager.getMediaPath(BeeBackend.Applications.kuikenTimer.configuration.background)
            backgroundOpacity: BeeBackend.Applications.kuikenTimer.configuration.backgroundOpacity
        }

        CountdownPanel {
            id: countdownTimer

            anchors.fill: parent
            backgroundSource: BeeBackend.Services.mediaManager.getMediaPath(BeeBackend.Applications.countdownTimer.configuration.background)
            backgroundOpacity: BeeBackend.Applications.countdownTimer.configuration.backgroundOpacity
            enabled: BeeBackend.Applications.countdownTimer.configuration.enabled
            initialized: BeeBackend.Applications.countdownTimer.configuration.initialized
            days: BeeBackend.Applications.countdownTimer.days
            hours: BeeBackend.Applications.countdownTimer.hours
            minutes: BeeBackend.Applications.countdownTimer.minutes
            seconds: BeeBackend.Applications.countdownTimer.seconds
            onClicked: menuOverlay.visible = true
        }
    }

    UpperMenuOverlay {
        id: menuOverlay

        visible: false
        anchors.fill: parent

        onClose: {
            menuOverlay.visible = false
            BeeBackend.Applications.menu.closeDialog()
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
