import QtQuick

import Components

Item {
	id: panel

	property PanelContainer panelContainer: parent

	property int showDelay: 0
	property int hideDelay: 0

	property real progress: 0.0
	property real progressScaleFactor: 0.075

	readonly property real invertedProgress: 1.0 - progress
	readonly property real direction: shown ? 1.0 : -1.0
	readonly property real progressScale: 1.0 + panel.invertedProgress * panel.direction * progressScaleFactor

	readonly property bool showing: transitionRunning && shown
	readonly property bool hiding: transitionRunning && !shown

	property bool shown: false
	property var transition: PanelTransition.fade
	property var previousTransition

	readonly property alias transitionRunning: animation.running

	signal aboutToShow()
	signal aboutToHide()

	signal fullyShown()
	signal fullyHidden()

	visible: shown || animation.running
	enabled: shown
	z: shown ? 1 : -1

	//anchors.centerIn: parent
	width: parent.width
	height: parent.height

	Component.onCompleted: {
		transition.enable(panel)
		previousTransition = transition
	}

	onTransitionChanged: {
		if (previousTransition) {
			previousTransition.disable(panel)
		}

		transition.enable(panel)
		previousTransition = transition
	}

	onShownChanged: {
		pauseAnimation.duration = panelContainer.visible ? (shown ? showDelay : hideDelay) : 0
		progress = shown ? 1.0 : 0.0
	}

	Behavior on progress {
		SequentialAnimation {
			id: animation

			PauseAnimation { id: pauseAnimation; duration: 0 }
			NumberAnimation { duration: panelContainer.visible ? panelContainer.duration : 0 }

			onRunningChanged: {
				if (running) {
					if (shown) {
						panel.aboutToShow()
					} else {
						panel.aboutToHide()
					}
				} else {
					if (shown) {
						panel.fullyShown()
					} else {
						panel.fullyHidden()
					}
				}
			}
		}
	}
}