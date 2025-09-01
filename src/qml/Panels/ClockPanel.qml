import QtQuick

import Components
import Bee

RoundPanel {
	id: clockPanel

    backgroundColor: Color.lightGray

    signal clicked()

    RoundAnimatedImage {
        id: clockBackgroundAnimation

        anchors.fill: parent
        
        source: AnimationManager.getAnimationPath(Backend.clock.backgroundAnimation)
        opacity: Backend.clock.backgroundOpacity
    }

    Clock {
        id: clock

        hourColor: Backend.clock.hourColor
        minuteColor: Backend.clock.minuteColor
        secondColor: Backend.clock.secondColor

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