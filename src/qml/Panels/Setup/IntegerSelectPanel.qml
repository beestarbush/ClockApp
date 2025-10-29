import QtQuick

import Components

RoundPanel {
    id: integerSelectPanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias descriptionText: description.text
    property alias valueText: value.text
    property bool valueTextSelected: false

    signal buttonClicked()
    signal valueSelected()

    Text {
        id: title

        width: parent.width - Value.largeMargin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: description.top
        anchors.bottomMargin: Value.defaultMargin
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
        anchors.bottom: value.top
        anchors.bottomMargin: Value.defaultMargin
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: value

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: valueTextSelected ? Color.green1 : Color.lightGray
        font.pixelSize: Value.largeTextSize
        font.bold: true
        horizontalAlignment: Text.AlignHCenter

        MouseArea {
            anchors.fill: parent
		    onClicked: integerSelectPanel.valueSelected()
        }
    }

    RoundButton {
        text: "Next"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: value.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: integerSelectPanel.buttonClicked()
    }
}