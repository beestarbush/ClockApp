import QtQuick

import Components
import Panels
import Bee as Backend

Item {
    id: content

    anchors.fill: parent

    UpperMainPanel {
        id: upperMainPanel

        width: Math.min(content.width, content.height)
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        lowerMenuOverlay: lowerMainPanel.menuOverlay
    }

    LowerMainPanel {
        id: lowerMainPanel

        width: Math.min(content.width, content.height) / 1.93
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }
    
    Loader {
        id: debugPanelLoader
        anchors.fill: parent
        active: Backend.Applications.debug.panelEnabled
        sourceComponent: debugPanelComponent

        onStatusChanged: {
            if (status === Loader.Ready) {
                console.log("Loaded debug panel")
            } else {
                console.log("Unloaded debug panel")
            }
        }
    }

    Component {
        id: debugPanelComponent
        DebugPanel {
            width: window.width
            height: window.height
            onCloseButtonClicked: debugPanelLoader.active = false
        }
    }
}