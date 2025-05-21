import QtQuick
import QtQuick.Layouts

import Components

Panel {
    id: dialog

    default property alias content: content.data
    property alias column: column

    property alias title: title.text
    property alias description: description.text
    property alias showBackground: background.visible
    property bool showCloseButton: false

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

    Column {
        id: column

        anchors.centerIn: parent
        spacing: Value.defaultMargin

        Text {
            id: title

            font.bold: true
            font.pixelSize: Value.largeTextSize
            anchors.horizontalCenter: parent.horizontalCenter

            color: Color.lightGray
        }

        Item {
            id: content

            anchors.horizontalCenter: parent.horizontalCenter
            width: childrenRect.width
            height: childrenRect.height
        }

        Text {
            id: description

            visible: text !== ""
            width: Value.defaultWidthRatio(dialog)
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap

            color: Color.lightGray
        }

        Button {
            id: closeButton

            anchors.horizontalCenter: parent.horizontalCenter
            visible: showCloseButton
            text: "close"

            onClicked: close()
        }
    }
}