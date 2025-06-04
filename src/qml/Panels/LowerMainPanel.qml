import QtQuick

import Components

Circle {
    id: lowerMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: pendulumPanel

        currentIndex: indexOfPanel(initialPanel)

        PendulumPanel {
            id: pendulumPanel

            //onDebugButtonClicked: panelContainer.showPanel(debugPanel)
        }
    }
}