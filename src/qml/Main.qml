import QtQuick

import Components
import Panels

Window {
	id: window

	width: 480
	height: 800
	visible: true
	title: qsTr("Clock application")
	color: Color.black

	Item {
		id: contentRoot

		anchors.fill: parent
		rotation: Backend.screenRotation

		Column {
			id: column
			anchors.topMargin: Value.defaultMargin
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.verticalCenter: parent.verticalCenter
			anchors.fill: parent
			spacing: Value.largeMargin

			UpperMainPanel {
				id: upperMainPanel

				width: Math.min(contentRoot.width, contentRoot.height) - (Value.defaultMargin * 2)
				height: width
				anchors.horizontalCenter: parent.horizontalCenter
			}
			LowerMainPanel {
				id: lowerMainPanel

				width: Math.min(contentRoot.width, contentRoot.height) / 2.5
				height: width
				anchors.horizontalCenter: parent.horizontalCenter
			}
		}
	}
}