import QtQuick

import Components

RoundPanel {
	id: clockPanel

    backgroundColor: Color.lightGray

    signal clicked()

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