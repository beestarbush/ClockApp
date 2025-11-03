import QtQuick

import Components
import Bee as BeeBackend

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

        // This menu shall be visible when: any menu dialog is active, or setup is not complete and a setup dialog is visible.
        visible: BeeBackend.Applications.menu.dialog !== BeeBackend.MenuEnums.None || 
                 (!BeeBackend.Applications.setup.setupComplete && 
                  (BeeBackend.Applications.setup.dialWheel.visible || BeeBackend.Applications.setup.mediaSelection.visible))
        anchors.fill: parent

        onClose: BeeBackend.Applications.menu.closeDialog()
    }
}