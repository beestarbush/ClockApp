import QtQuick
import QtQuick.Layouts

import Components

RoundPanel {
    id: timeSincePanel

    backgroundColor: Color.lightGray

    property alias happening: happeningLabel.text
    property int days
    property int hours
    property int minutes
    property int seconds
    signal clicked()

    ColumnLayout {
        id: column

        anchors.fill: parent
        anchors.centerIn: parent
        spacing: Value.defaultMargin

        Item {
            Layout.fillHeight: true
        }

        Text {
            id: happeningLabel

            Layout.alignment: Qt.AlignHCenter
            font.bold: true
            font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.10 : Value.defaultTextSize
        }

        Text {
            id: daysLabel

            Layout.alignment: Qt.AlignHCenter
            font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
            text: "%1 dagen".arg(days)
        }
        Text {
            id: hoursLabel

            Layout.alignment: Qt.AlignHCenter
            font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
            text: "%1 uren".arg(hours)
        }
        Text {
            id: minutesLabel

            Layout.alignment: Qt.AlignHCenter
            font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
            text: "%1 minuten".arg(minutes)
        }
        Text {
            id: secondsLabel

            Layout.alignment: Qt.AlignHCenter
            font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
            text: "%1 seconden".arg(seconds)
        }

        Item {
            Layout.fillHeight: true
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            timeSincePanel.clicked()
            console.log("timesince Clicked")
        }
    }
}