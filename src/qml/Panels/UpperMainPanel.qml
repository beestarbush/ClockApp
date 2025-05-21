import QtQuick

import Components

Circle {
    id: upperMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel targetPanel: clockPanel

        currentIndex: indexOfPanel(targetPanel)

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
            days: 201
            hours: 13
            minutes: 56
            seconds: 21
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
            if (direction) {
                direction = panelContainer.nextPanel()
            }
            else {
                direction = !panelContainer.previousPanel()
            }
        }
    }
}