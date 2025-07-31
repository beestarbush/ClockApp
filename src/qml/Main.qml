import QtQuick

import Components
import Panels

Window {
	id: window

	width: 720
	height: 1280
	visible: true
	title: qsTr("Clock application")
	color: Color.black

	Item {
		id: contentRoot

		anchors.fill: parent

		Column {
			id: column
			anchors.topMargin: Value.defaultMargin
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.verticalCenter: parent.verticalCenter
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
