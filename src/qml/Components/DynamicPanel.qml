import QtQuick

import Components

Panel {
	id: panel

    default property alias content: loader.sourceComponent
    readonly property bool loaded: status === Loader.Ready
    property alias loadingRequest: loader.active
    property alias status: loader.status
    property alias asynchronous: loader.asynchronous
    property alias dynamicContent: loader.item
    property bool keepLoaded: false

    Loader {
        id: loader

        active: panel.visible

        asynchronous: true
        anchors.fill: parent

        onLoaded: {
            if (keepLoaded) {
                active = true
            }
        }
    }
}