import QtQuick

import Components

Rectangle {
	id: button

	width: 120
	height: 40
	radius: 6
	color: Color.gray

	property alias text: label.text
	property alias buttonColor: button.color
	signal clicked()
	signal pressed()
	signal released()

	Text {
		id: label
		anchors.centerIn: parent
		color: Color.white
		font.pixelSize: 16
		wrapMode: Text.Wrap
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}
	MouseArea {
		anchors.fill: parent
		onClicked: button.clicked()
		onPressed: button.pressed()
		onReleased: button.released()
	}
}