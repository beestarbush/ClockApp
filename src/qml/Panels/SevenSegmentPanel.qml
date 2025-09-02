import QtQuick

import Components
import Bee

RoundPanel {
    id: sevenSegmentPanel

    property int years
    property int weeks
    property alias days: daysSegments.number
    property alias hours: hoursSegments.number
    property alias minutes: minutesSegments.number
    property alias seconds: secondsSegments.number

    property real daysToTimeAspectRatio: 0.5
    
    backgroundColor: Color.black

    signal clicked()

    RoundAnimatedImage {
        id: backgroundAnimation

        anchors.fill: parent
        
        source: AnimationManager.getAnimationPath(Backend.marriedTimer.backgroundAnimation)
        opacity: Backend.marriedTimer.backgroundOpacity
    }
    
    Item {
        id: daysSegments
        property int number: 0
        property int digitCount: 5
        property color segmentColor: "white"
        property int spacing: 10

        anchors.centerIn: parent

        width: digitCount * (digitWidth + spacing)
        height: digitHeight

        property int digitWidth: 100
        property int digitHeight: 200

        Row {
            spacing: daysSegments.spacing

            Repeater {
                model: daysSegments.digitCount
                SevenSegmentDisplay {
                    width: daysSegments.digitWidth
                    height: daysSegments.digitHeight
                    digit: parseInt(daysSegments.numberString.charAt(index))
                    color: daysSegments.segmentColor
                }
            }
        }

        property string numberString: {
            var str = number.toString()
            while (str.length < digitCount)
                str = "0" + str
            return str
        }
    }

    Item {
        id: hoursSegments
        property int number: 0
        property int digitCount: 2
        property color segmentColor: "white"
        property int spacing: 10

        anchors.top: daysSegments.bottom
        anchors.left: daysSegments.left
        anchors.leftMargin: Value.defaultMargin

        width: digitCount * (digitWidth + spacing)
        height: digitHeight

        property int digitWidth: daysSegments.digitWidth * daysToTimeAspectRatio
        property int digitHeight: daysSegments.digitHeight * daysToTimeAspectRatio

        Row {
            spacing: hoursSegments.spacing

            Repeater {
                model: hoursSegments.digitCount
                SevenSegmentDisplay {
                    width: hoursSegments.digitWidth
                    height: hoursSegments.digitHeight
                    digit: parseInt(hoursSegments.numberString.charAt(index))
                    color: hoursSegments.segmentColor
                }
            }
        }

        property string numberString: {
            var str = number.toString()
            while (str.length < digitCount)
                str = "0" + str
            return str
        }
    }

    Item {
        id: minutesSegments
        property int number: 0
        property int digitCount: 2
        property color segmentColor: "white"
        property int spacing: 10

        anchors.top: daysSegments.bottom
        anchors.horizontalCenter: daysSegments.horizontalCenter

        width: digitCount * (digitWidth + spacing)
        height: digitHeight

        property int digitWidth: daysSegments.digitWidth * daysToTimeAspectRatio
        property int digitHeight: daysSegments.digitHeight * daysToTimeAspectRatio

        Row {
            spacing: minutesSegments.spacing

            Repeater {
                model: minutesSegments.digitCount
                SevenSegmentDisplay {
                    width: minutesSegments.digitWidth
                    height: minutesSegments.digitHeight
                    digit: parseInt(minutesSegments.numberString.charAt(index))
                    color: minutesSegments.segmentColor
                }
            }
        }

        property string numberString: {
            var str = number.toString()
            while (str.length < digitCount)
                str = "0" + str
            return str
        }
    }

    Item {
        id: secondsSegments
        property int number: 0
        property int digitCount: 2
        property color segmentColor: "white"
        property int spacing: 10

        anchors.top: daysSegments.bottom
        anchors.right: daysSegments.right
        anchors.rightMargin: Value.defaultMargin

        width: digitCount * (digitWidth + spacing)
        height: digitHeight

        property int digitWidth: daysSegments.digitWidth * daysToTimeAspectRatio
        property int digitHeight: daysSegments.digitHeight * daysToTimeAspectRatio

        Row {
            spacing: secondsSegments.spacing

            Repeater {
                model: secondsSegments.digitCount
                SevenSegmentDisplay {
                    width: secondsSegments.digitWidth
                    height: secondsSegments.digitHeight
                    digit: parseInt(secondsSegments.numberString.charAt(index))
                    color: secondsSegments.segmentColor
                }
            }
        }

        property string numberString: {
            var str = number.toString()
            while (str.length < digitCount)
                str = "0" + str
            return str
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            sevenSegmentPanel.clicked()
        }
    }
}