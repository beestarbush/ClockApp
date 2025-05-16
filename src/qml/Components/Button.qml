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

	Text {
		id: label
		anchors.centerIn: parent
		color: Color.white
		font.pixelSize: 16
	}
	MouseArea {
		anchors.fill: parent
		onClicked: button.clicked()
	}
}