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

        DebugPanel {
            id: debugPanel

            anchors.fill: parent

            onCloseButtonClicked: panelContainer.showPanel(clockPanel)
        }

        ClockPanel {
            id: clockPanel

            anchors.fill: parent

            onClicked: panelContainer.showPanel(debugPanel)
        }
    }
}