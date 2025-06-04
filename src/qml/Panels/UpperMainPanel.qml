import QtQuick

import Components

Circle {
    id: upperMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: clockPanel

        currentIndex: indexOfPanel(initialPanel)

        /*DebugPanel {
            id: debugPanel

            anchors.fill: parent

            onCloseButtonClicked: panelContainer.showPanel(clockPanel)
        }*/

        ClockPanel {
            id: clockPanel

            anchors.fill: parent

            onClicked: dialogOverlay.visible = true
        }

        TimeSincePanel {
            id: timeSinceMarriedPanel

            anchors.fill: parent
            onClicked: dialogOverlay.visible = true
            happening: "Getrouwd"
            years: 0
            days: 0
            hours: 0
            minutes: 0
            seconds: 0
        }

        TimeSincePanel {
            id: timeSinceKuikenPanel

            anchors.fill: parent
            onClicked: dialogOverlay.visible = true
            happening: "Kuiken"
            showYearsInCenter: false
            years: 0
            days: 0
            daysDivisor: 7
            hours: 0
            minutes: 0
            seconds: 0
        }
    }

    DialogOverlay {
        id: dialogOverlay

        visible: false
        anchors.fill: parent

        onClose: dialogOverlay.visible = false
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

        property var referenceDate: new Date(1730382722 * 1000)

        onTriggered: {
            var now = new Date();
            var diff = now - referenceDate // milliseconds
            var tempDate = new Date(referenceDate);
            var years = 0;
            tempDate.setFullYear(referenceDate.getFullYear() + years);

            // If we haven't reached the anniversary yet this year, subtract one year
            if (now < tempDate) {
                years--;
                tempDate.setFullYear(referenceDate.getFullYear() + years);
            }

            // Days since last full year
            var days = Math.ceil((now - tempDate) / (1000 * 60 * 60 * 24));
            tempDate.setDate(tempDate.getDate() + days);

            // Hours, minutes, seconds since last full day
            var hours = now.getHours() - tempDate.getHours();
            var minutes = now.getMinutes() - tempDate.getMinutes();
            var seconds = now.getSeconds() - tempDate.getSeconds();

            if (seconds < 0) {
                seconds += 60;
                minutes--;
            }
            if (minutes < 0) {
                minutes += 60;
                hours--;
            }
            if (hours < 0) {
                hours += 24;
                days--;
            }

            // Update the panel
            timeSinceMarriedPanel.years = years;
            timeSinceMarriedPanel.days = days;
            timeSinceMarriedPanel.hours = hours;
            timeSinceMarriedPanel.minutes = minutes;
            timeSinceMarriedPanel.seconds = seconds;
        }
    }

    Timer {
        id: timeSinceKuikenTimer

        interval: 1000
        running: true
        repeat: true

        property var referenceDate: new Date(2025, 1, 6, 0, 0, 0)

        onTriggered: {
            var now = new Date();
            var diff = now - referenceDate;

            // Total days since reference date
            var totalDays = Math.floor((now - referenceDate) / (1000 * 60 * 60 * 24));
            var years = 0;
            var weeks = Math.ceil(totalDays / 7);
            var days = totalDays % 7;

            // Calculate the time since the last full day
            var tempDate = new Date(referenceDate);
            tempDate.setDate(tempDate.getDate() + totalDays);

            var hours = now.getHours() - tempDate.getHours();
            var minutes = now.getMinutes() - tempDate.getMinutes();
            var seconds = now.getSeconds() - tempDate.getSeconds();

            if (seconds < 0) {
                seconds += 60;
                minutes--;
            }
            if (minutes < 0) {
                minutes += 60;
                hours--;
            }
            if (hours < 0) {
                hours += 24;
                days--;
                if (days < 0) {
                    days += 7;
                    weeks--;
                }
            }

            // Update the panel
            timeSinceKuikenPanel.years = years;
            timeSinceKuikenPanel.weeks = weeks
            timeSinceKuikenPanel.days = days;
            timeSinceKuikenPanel.hours = hours;
            timeSinceKuikenPanel.minutes = minutes;
            timeSinceKuikenPanel.seconds = seconds;
        }
    }
}