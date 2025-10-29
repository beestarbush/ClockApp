import QtQuick

import Components

Circle {
    id: lowerMainPanel

    color: Color.blue // This should not be visible, if it is then some formatting is wrong.
    property alias menuOverlay: menuOverlay

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: pendulumPanel

        currentIndex: indexOfPanel(initialPanel)

        PendulumPanel {
            id: pendulumPanel
        }
    }

    LowerMenuOverlay {
        id: menuOverlay

        visible: false
        anchors.fill: parent

        onClose: menuOverlay.visible = false
    }
}