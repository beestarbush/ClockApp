import QtQuick

import Components

RoundPanel {
    id: dateTimePickerPanel

    anchors.fill: parent
    backgroundColor: Color.transparent

    property alias titleText: title.text
    property alias descriptionText: description.text
    property alias dateTime: dateTimePicker.selectedDateTime
    property alias selectedComponent: dateTimePicker.selectedComponent

    signal buttonClicked()
    signal componentSelected(int component)

    function updateSelectedComponent(newValue) {
        dateTimePicker.updateSelectedComponent(newValue)
    }

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
        anchors.bottom: dateTimePicker.top
        anchors.bottomMargin: Value.defaultMargin
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    DateTimePicker {
        id: dateTimePicker
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        
        width: parent.width * 0.8
        height: implicitHeight
        
        selectedTextColor: Color.green1
        defaultTextColor: Color.lightGray
        
        onComponentSelected: function(component) {
            dateTimePickerPanel.componentSelected(component)
        }
    }

    RoundButton {
        text: "Next"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dateTimePicker.bottom
        anchors.topMargin: Value.defaultMargin

        onClicked: dateTimePickerPanel.buttonClicked()
    }
}