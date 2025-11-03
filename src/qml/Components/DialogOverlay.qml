import QtQuick
import QtQuick.Controls

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(menuDialog)

    signal close()

    MenuDialog {
        id: menuDialog

        RoundLayout {
            anchors.fill: parent
        }
    }
}