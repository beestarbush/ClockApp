import QtQuick

import Components

RoundPanel {
    id: timeSincePanel

    property alias happening: happeningLabel.text
    property int years
    property int weeks
    property int days
    property int daysDivisor: 365
    property int hours
    property int minutes
    property int seconds
    property bool showYearsInCenter: true

    backgroundColor: Color.lightGray
    property int barThickness: width / 15

    signal clicked()

    RoundProgressBar {
        id: secondsProgressBar

        anchors.centerIn: parent
        anchors.fill: parent
        color: Color.green1
        thickness: barThickness
        showLabel: false

        progress: seconds
        divisor: 60
    }

    RoundProgressBar {
        id: minutesProgressBar

        anchors.centerIn: parent
        width: secondsProgressBar.width - (barThickness * 2)
        height: secondsProgressBar.height - (barThickness * 2)
        color: Color.blue
        thickness: barThickness

        progress: minutes
        divisor: 60
    }
    RoundProgressBar {
        id: hoursProgressBar

        anchors.centerIn: parent
        width: minutesProgressBar.width - (barThickness * 2)
        height: minutesProgressBar.height - (barThickness * 2)
        color: Color.red
        thickness: barThickness

        progress: hours
        divisor: 24
    }
    RoundProgressBar {
        id: daysProgressBar

        anchors.centerIn: parent
        width: hoursProgressBar.width - (barThickness * 2)
        height: hoursProgressBar.height - (barThickness * 2)
        color: Color.yellow
        thickness: barThickness

        progress: days
        divisor: daysDivisor
    }
    Text {
        id: happeningLabel

        anchors.bottom: centerValue.top
        anchors.horizontalCenter: centerValue.horizontalCenter
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
    }
    Text {
        id: centerValue

        anchors.centerIn: parent
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.10 : Value.defaultTextSize
        font.bold: true
        text: showYearsInCenter ? years : weeks
    }
    Text {
        id: centerLabel

        anchors.top: centerValue.bottom
        anchors.horizontalCenter: centerValue.horizontalCenter
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.05 : Value.defaultTextSize
        text: showYearsInCenter ? "jaar" : "weken"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            timeSincePanel.clicked()
            console.log("timesince Clicked")
        }
    }
}