import QtQuick

import Components

RoundPanel {
    id: titlePanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias descriptionText: description.text
    property alias buttonText: button.text

    signal buttonClicked()

    Text {
        id: title

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: description.top
        anchors.bottomMargin: Value.defaultMargin
        color: Color.lightGray
        font.pixelSize: Value.largeTextSize
        font.bold: true
    }

    Text {
        id: description

        width: parent.width - Value.largeMargin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    RoundButton {
        id: button

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: description.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: titlePanel.buttonClicked()
    }
}