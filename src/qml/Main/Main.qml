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

	signal frontendReady()

	onFrontendReady: panelContainer.showPanel(contentPanel)

	 PanelContainer {
        id: panelContainer

        anchors.fill: parent
		anchors.top: parent.top

        property Panel initialPanel: startupPanel

        currentIndex: indexOfPanel(initialPanel)

		StartupPanel {
			id: startupPanel

			width: Math.min(parent.width, parent.height)
			height: width

			anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter

			Component.onCompleted: contentPanel.load()
		}

		DynamicPanel {
			id: contentPanel

			property date loadingTime
			
			keepLoaded: true

			anchors.fill: parent
			content: contentComponent

			onStatusChanged: {
				if (status == Loader.Loading) {
					console.log("Start loading content")
					loadingTime = new Date()
				} else if (status == Loader.Ready) {
					var loadingDuration = new Date().getTime() - loadingTime.getTime()
					console.log("Loaded content, took: " + loadingDuration + "ms")
					frontendReady()
				}
			}

			function load() {
				contentPanel.content = Qt.createComponent("Main", "Content")
				console.assert(contentPanel.content.status == Component.Ready)
				loadingRequest = true
			}
		}
	 }
}
