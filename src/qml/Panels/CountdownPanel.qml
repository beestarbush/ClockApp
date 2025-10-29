import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: countdownPanel

    backgroundColor: Color.black
    property alias backgroundSource: background.source
    property alias backgroundOpacity: background.opacity
    property alias days: daysText.text
    property alias hours: hoursText.text
    property alias minutes: minutesText.text
    property alias seconds: secondsText.text
    property bool initialized

    signal clicked()

    BeeBackend.RoundAnimatedImage {
        id: background

        anchors.fill: parent
    }

    // Container for progress bars - shown when initialized
    Item {        
        anchors.fill: parent
        visible: countdownPanel.initialized

        Row {
            anchors.centerIn: parent
            spacing: Value.defaultMargin

            Text {
                id: daysText
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Value.largeTextSize
                color: Color.lightGray
            }
            Text {
                id: hoursText
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Value.largeTextSize
                color: Color.lightGray
            }
            Text {
                id: minutesText
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Value.largeTextSize
                color: Color.lightGray
            }
            Text {
                id: secondsText
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Value.largeTextSize
                color: Color.lightGray
            }
        }
    }

    // Container for "not set up" message - shown when not initialized
    Item {        
        anchors.fill: parent
        visible: !countdownPanel.initialized
        
        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            text: "Timer not set up yet"
            font.pixelSize: Value.largeTextSize
            font.bold: true
            color: Color.lightGray
        }
    }

    MouseArea {
		anchors.fill: parent
		onClicked: clockPanel.clicked()
	}
}