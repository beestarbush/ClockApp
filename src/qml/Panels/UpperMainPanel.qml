import QtQuick

import Components
import Bee as Bee

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
            enabled: Backend.clockEnabled

            onClicked: menuOverlay.visible = true
        }

        SevenSegmentPanel {
            id: timeSinceMarriedPanel

            anchors.fill: parent
            enabled: Backend.marriedTimerEnabled
            onClicked: menuOverlay.visible = true
            years: Bee.TimeSinceMarried.years
            days: Bee.TimeSinceMarried.days
            hours: Bee.TimeSinceMarried.hours
            minutes: Bee.TimeSinceMarried.minutes
            seconds: Bee.TimeSinceMarried.seconds
        }

        TimeSincePanel {
            id: timeSinceKuikenPanel

            anchors.fill: parent
            enabled: Backend.kuikenTimerEnabled
            onClicked: menuOverlay.visible = true
            happening: "Kuiken"
            showYearsInCenter: false
            years: Bee.TimeSinceKuiken.years
            days: Bee.TimeSinceKuiken.days
            daysDivisor: 7
            hours: Bee.TimeSinceKuiken.hours
            minutes: Bee.TimeSinceKuiken.minutes
            seconds: Bee.TimeSinceKuiken.seconds
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

    Timer {
        id: timeSinceMarriedTimer

        interval: 1000
        running: true
        repeat: true

        onTriggered: {
            timeSinceMarriedPanel.years = Bee.TimeSinceMarried.years;
            timeSinceMarriedPanel.days = Bee.TimeSinceMarried.days;
            timeSinceMarriedPanel.hours = Bee.TimeSinceMarried.hours;
            timeSinceMarriedPanel.minutes = Bee.TimeSinceMarried.minutes;
            timeSinceMarriedPanel.seconds = Bee.TimeSinceMarried.seconds;
        }
    }

    Timer {
        id: timeSinceKuikenTimer

        interval: 1000
        running: true
        repeat: true

        onTriggered: {
            timeSinceKuikenPanel.years = Bee.TimeSinceKuiken.years;
            timeSinceKuikenPanel.weeks = Bee.TimeSinceKuiken.weeks
            timeSinceKuikenPanel.days = Bee.TimeSinceKuiken.days;
            timeSinceKuikenPanel.hours = Bee.TimeSinceKuiken.hours;
            timeSinceKuikenPanel.minutes = Bee.TimeSinceKuiken.minutes;
            timeSinceKuikenPanel.seconds = Bee.TimeSinceKuiken.seconds;
        }
    }
}
