import QtQuick

import Components
import Bee as Backend

RoundPanel {
	id: clockPanel

    backgroundColor: Color.black

    signal clicked()

    Backend.RoundAnimatedImage {
        id: clockBackground

        anchors.fill: parent
        
        source: Backend.Services.media.getMediaPath(Backend.Applications.clock.configuration.background)
        opacity: Backend.Applications.clock.configuration.backgroundOpacity
    }

    Clock {
        id: clock

        hourColor: Backend.Applications.clock.configuration.hourColor
        minuteColor: Backend.Applications.clock.configuration.minuteColor
        secondColor: Backend.Applications.clock.configuration.secondColor

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