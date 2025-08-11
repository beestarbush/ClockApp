import QtQuick
import QtQuick.Layouts

import Components

Panel {
    id: dialog

    property alias backgroundOpacity: background.opacity
    signal close()

    transition: PanelTransition.scale

    MouseArea {
        id: clickInhibitor

        anchors.fill: parent
    }

    Circle {
        id: background
        anchors.fill: parent
        opacity: 0.6
        color: Color.black
        scale: 1.0 / dialog.scale
    }
}