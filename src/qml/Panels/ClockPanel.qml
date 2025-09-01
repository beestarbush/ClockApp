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
        source: AnimationManager.getAnimationPath(Backend.clockBackgroundAnimation)
        opacity: Backend.clockBackgroundAnimationOpacity
    }

    Clock {
        id: clock

        hourColor: Backend.hourColor
        minuteColor: Backend.minuteColor
        secondColor: Backend.secondColor

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