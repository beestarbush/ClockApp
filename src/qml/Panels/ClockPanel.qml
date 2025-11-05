import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: clockPanel

    backgroundColor: Color.black

    signal clicked()

    BeeBackend.RoundAnimatedImage {
        id: clockBackground

        anchors.fill: parent
        
        source: BeeBackend.Services.media.getMediaPath(BeeBackend.Applications.clock.configuration.background)
        opacity: BeeBackend.Applications.clock.configuration.backgroundOpacity
    }

    Clock {
        id: clock

        hourColor: BeeBackend.Applications.clock.configuration.hourColor
        minuteColor: BeeBackend.Applications.clock.configuration.minuteColor
        secondColor: BeeBackend.Applications.clock.configuration.secondColor

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