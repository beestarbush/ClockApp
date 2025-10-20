import QtQuick

import Components

RoundPanel {
    id: dateTimePickerPanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias dateTime: dateTimePicker.selectedDateTime

    signal buttonClicked()

    Text {
        id: title

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Color.lightGray
        font.pixelSize: Value.largeTextSize
        font.bold: true
    }

    DateTimePicker {
        id: dateTimePicker
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: Value.defaultMargin
        
        width: parent.width * 0.8
        height: implicitHeight
        
        selectedTextColor: Color.green1
        defaultTextColor: Color.lightGray

        onDateTimeChanged: function(newDateTime) {
            // Handle datetime changes if needed
        }
        
        onComponentSelected: function(component) {
            // Handle component selection if needed
            // Could update a dial wheel here
        }
    }

    Button {
        text: "Next"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dateTimePicker.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: dateTimePickerPanel.buttonClicked()
    }
}