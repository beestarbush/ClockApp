import QtQuick

import Components
import Bee

RoundPanel {
    id: timeSincePanel

    property int years
    property int weeks
    property int days
    property int daysInWeek
    property int hours
    property int minutes
    property int seconds
    property color barColor: Backend.kuikenTimer.barColor
    property color textColor: Backend.kuikenTimer.textColor

    backgroundColor: Color.black
    property int barThickness: width / 35

    signal clicked()

    RoundAnimatedImage {
        id: backgroundAnimation

        anchors.fill: parent
        
        source: AnimationManager.getAnimationPath(Backend.kuikenTimer.backgroundAnimation)
        opacity: Backend.kuikenTimer.backgroundOpacity
    }

    RoundProgressBar {
        id: secondsProgressBar

        anchors.centerIn: parent
        anchors.fill: parent
        color: Qt.darker(timeSincePanel.barColor, 1.9)
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
        color: Qt.darker(timeSincePanel.barColor, 1.6)
        thickness: barThickness

        progress: minutes
        divisor: 60
    }
    RoundProgressBar {
        id: hoursProgressBar

        anchors.centerIn: parent
        width: minutesProgressBar.width - (barThickness * 2)
        height: minutesProgressBar.height - (barThickness * 2)
        color: Qt.darker(timeSincePanel.barColor, 1.3)
        thickness: barThickness

        progress: hours
        divisor: 24
    }
    RoundProgressBar {
        id: daysProgressBar

        anchors.centerIn: parent
        width: hoursProgressBar.width - (barThickness * 2)
        height: hoursProgressBar.height - (barThickness * 2)
        color: timeSincePanel.barColor
        thickness: barThickness

        progress: years > 0 ? (days % 365) : daysInWeek
        divisor: years > 0 ? 365 : 7
    }
    Text {
        id: centerValue

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.10 : Value.defaultTextSize
        color: timeSincePanel.textColor
        font.bold: true
        text: years > 0 ? years : weeks
    }
    Text {
        id: centerLabel

        anchors.top: centerValue.bottom
        anchors.horizontalCenter: centerValue.horizontalCenter
        color: timeSincePanel.textColor
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.10 : Value.defaultTextSize
        text: years > 0 ? "jaar" : "weken"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: timeSincePanel.clicked()
    }
}