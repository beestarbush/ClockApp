import QtQuick

import Components

Rectangle {
	id: button

	width: 120
	height: 40
	radius: 6
	color: isPressed ? Qt.darker(Color.green1, 1.3) : Color.green1

	property alias text: label.text
	property alias buttonColor: button.color
	property bool isPressed: false

	signal clicked()
	signal pressed()
	signal released()

	Text {
		id: label
		anchors.centerIn: parent
		width: parent.width - Value.smallMargin
		color: Color.white
		font.pixelSize: 16
		wrapMode: Text.Wrap
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}
	MouseArea {
		anchors.fill: parent
		onClicked: button.clicked()
		onPressed: {
			isPressed = true
			button.pressed()
		}
		onReleased: {
			isPressed = false
			button.released()
		}
	}
}