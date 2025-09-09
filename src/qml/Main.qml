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

		UpperMainPanel {
			id: upperMainPanel

			width: Math.min(contentRoot.width, contentRoot.height)
			height: width
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.top: parent.top

			lowerMenuOverlay: lowerMainPanel.menuOverlay
		}

		LowerMainPanel {
			id: lowerMainPanel

			width: Math.min(contentRoot.width, contentRoot.height) / 1.93
			height: width
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.bottom
		}
	}

	DebugPanel {
		id: debugPanel

		width: window.width
		height: window.height
		visible: Backend.debugging.panelEnabled
		onCloseButtonClicked: debugPanel.visible = false
	}
}
