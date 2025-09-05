import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
    id: roundProgressBarPanel

    property int years
    property int weeks
    property int days
    property int daysInWeek
    property int hours
    property int minutes
    property int seconds
    property color barColor
    property color textColor
    property alias backgroundAnimationSource: backgroundAnimation.source
    property alias backgroundAnimationOpacity: backgroundAnimation.opacity

    backgroundColor: Color.black
    property int barThickness: width / 35

    signal clicked()

    BeeBackend.RoundAnimatedImage {
        id: backgroundAnimation

        anchors.fill: parent
    }

    RoundProgressBar {
        id: secondsProgressBar

        anchors.centerIn: parent
        anchors.fill: parent
        color: Qt.darker(roundProgressBarPanel.barColor, 1.9)
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
        color: Qt.darker(roundProgressBarPanel.barColor, 1.6)
        thickness: barThickness

        progress: minutes
        divisor: 60
    }
    RoundProgressBar {
        id: hoursProgressBar

        anchors.centerIn: parent
        width: minutesProgressBar.width - (barThickness * 2)
        height: minutesProgressBar.height - (barThickness * 2)
        color: Qt.darker(roundProgressBarPanel.barColor, 1.3)
        thickness: barThickness

        progress: hours
        divisor: 24
    }
    RoundProgressBar {
        id: daysProgressBar

        anchors.centerIn: parent
        width: hoursProgressBar.width - (barThickness * 2)
        height: hoursProgressBar.height - (barThickness * 2)
        color: roundProgressBarPanel.barColor
        thickness: barThickness

        progress: years > 0 ? (days % 365) : daysInWeek
        divisor: years > 0 ? 365 : 7
    }
    Text {
        id: centerValue

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        font.pointSize: roundProgressBarPanel.width > 0 ? roundProgressBarPanel.width * 0.10 : Value.defaultTextSize
        color: roundProgressBarPanel.textColor
        font.bold: true
        text: years > 0 ? years : weeks
    }
    Text {
        id: centerLabel

        anchors.top: centerValue.bottom
        anchors.horizontalCenter: centerValue.horizontalCenter
        color: roundProgressBarPanel.textColor
        font.pointSize: roundProgressBarPanel.width > 0 ? roundProgressBarPanel.width * 0.10 : Value.defaultTextSize
        text: years > 0 ? "jaar" : "weken"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: roundProgressBarPanel.clicked()
    }
}