import QtQuick

import Components

RoundPanel {
    id: toggleButtonPanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias descriptionText: description.text
    property alias toggleTarget: toggleButton.target
    property alias toggleProperty: toggleButton.property

    signal buttonClicked()

    Text {
        id: title

        width: parent.width - Value.largeMargin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Color.lightGray
        font.pixelSize: Value.largeTextSize
        font.bold: true
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: description

        width: parent.width - Value.largeMargin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: Value.defaultMargin
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    ToggleButton {
        id: toggleButton

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: description.bottom
        anchors.topMargin: Value.defaultMargin

        enabled: true
        size: ToggleButton.Size.Large
    }

    RoundButton {
        text: "Next"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: toggleButton.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: toggleButtonPanel.buttonClicked()
    }
}