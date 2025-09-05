import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: clockPanel

    backgroundColor: Color.lightGray

    signal clicked()

    BeeBackend.RoundAnimatedImage {
        id: clockBackgroundAnimation

        anchors.fill: parent
        
        source: BeeBackend.Applications.clock.backgroundAnimation
        opacity: BeeBackend.Applications.clock.backgroundOpacity
    }

    Clock {
        id: clock

        hourColor: BeeBackend.Applications.clock.hourColor
        minuteColor: BeeBackend.Applications.clock.minuteColor
        secondColor: BeeBackend.Applications.clock.secondColor

        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - (Value.defaultMargin * 2)
        height: width
    }

    MouseArea {
		anchors.fill: parent
		onClicked: clockPanel.clicked()
	}
}