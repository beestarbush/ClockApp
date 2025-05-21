import QtQuick

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(settingsDialog)

    signal close()

    Dialog {
        id: settingsDialog

        title: "Settings"
        description: "this contains settings"
        showCloseButton: true

        onClose: dialogOverlay.close()
    }
}