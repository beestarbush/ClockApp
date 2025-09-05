import QtQuick

import Components
import Bee as BeeBackend

Circle {
    id: upperMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.
    property alias lowerMenuOverlay: menuOverlay.lowerMenuOverlay

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: clockPanel

        currentIndex: indexOfPanel(initialPanel)

        ClockPanel {
            id: clockPanel

            anchors.fill: parent
            enabled: BeeBackend.Applications.clock.enabled

            onClicked: menuOverlay.visible = true
        }

        SevenSegmentPanel {
            id: marriedPanel

            anchors.fill: parent
            enabled: BeeBackend.Applications.marriedTimer.enabled
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
            enabled: BeeBackend.Applications.kuikenTimer.enabled
            onClicked: menuOverlay.visible = true
            years: BeeBackend.Applications.kuikenTimer.years
            days: BeeBackend.Applications.kuikenTimer.days
            daysInWeek: BeeBackend.Applications.kuikenTimer.daysInWeek
            weeks: BeeBackend.Applications.kuikenTimer.weeks
            hours: BeeBackend.Applications.kuikenTimer.hours
            minutes: BeeBackend.Applications.kuikenTimer.minutes
            seconds: BeeBackend.Applications.kuikenTimer.seconds
            barColor: BeeBackend.Applications.kuikenTimer.barColor
            textColor: BeeBackend.Applications.kuikenTimer.textColor
            backgroundAnimationSource: BeeBackend.Applications.kuikenTimer.backgroundAnimation
            backgroundAnimationOpacity: BeeBackend.Applications.kuikenTimer.backgroundOpacity
        }
    }

    UpperMenuOverlay {
        id: menuOverlay

        visible: false
        anchors.fill: parent

        onClose: menuOverlay.visible = false

        onVisibleChanged:  {
            lowerMenuOverlay.visible = menuOverlay.visible
        }
    }

    Timer {
        id: panelRotationTimer

        interval: 10000 // 10 seconds
        running: true
        repeat: true

        property bool direction: true
        onTriggered: {
            if (!panelContainer.nextPanel()) {
                panelContainer.showPanel(panelContainer.initialPanel);
            }
        }
    }
}
