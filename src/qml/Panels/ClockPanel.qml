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
        source: "qrc:/animations/test.gif"
        opacity: Backend.clockBackgroundAnimationOpacity
    }

    Clock {
        id: clock

        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - (Value.defaultMargin * 2)
        height: width
    }

    MouseArea {
		anchors.fill: parent
		onClicked: {
            clockPanel.clicked()
            console.log("clockPanel Clicked")
        }
	}
}