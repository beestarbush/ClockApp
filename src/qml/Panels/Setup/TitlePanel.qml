import QtQuick

import Components

RoundPanel {
    id: titlePanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias buttonText: button.text

    signal buttonClicked()

    Text {
        id: title

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Color.lightGray
        font.pixelSize: Value.largeTextSize
        font.bold: true
    }

    Button {
        id: button

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: titlePanel.buttonClicked()
    }
}