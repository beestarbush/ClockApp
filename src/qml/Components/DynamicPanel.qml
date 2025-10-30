import QtQuick

import Components

Panel {
	id: panel

    property alias content: loader.sourceComponent
    property alias active: loader.active

    signal loaded()

    Loader {
        id: loader
        anchors.fill: parent

        onStatusChanged: {
            if (loader.status == Loader.Ready) {
                panel.loaded()
            }
        }
    }
}