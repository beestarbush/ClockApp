import QtQuick
import Components
import Panels

Window {
	id: window

	width: 600
	height: 1024
	visible: true
	title: qsTr("Hello World")
	color: Color.black

	Column {
		id: column
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		spacing: Value.largeMargin

		UpperMainPanel {
			id: upperMainPanel

			width: window.width - (Value.defaultMargin * 2)
			height: width
			anchors.horizontalCenter: parent.horizontalCenter
		}
		LowerMainPanel {
			id: lowerMainPanel

			width: window.width / 2.5
			height: width
			anchors.horizontalCenter: parent.horizontalCenter
		}
	}
}