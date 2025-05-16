import QtQuick

import Components

Item {
	id: panelContainer

	property int duration: Value.defaultAnimationDuration
	property int currentIndex: 0
	readonly property alias previousIndex: internal.previousIndex
	readonly property Item currentPanel: (currentIndex >= 0) ? panelContainer.children[currentIndex] : null
	readonly property int panelCount: children.length
	readonly property bool firstPanel: currentIndex === 0
	readonly property bool lastPanel: currentIndex === children.length - 1
	readonly property real direction: currentIndex > previousIndex ? 1 : -1

	readonly property bool transitionRunning: currentPanel && currentPanel.transitionRunning

	function previousPanel() {
		if (firstPanel) {
			return false
		}

		currentIndex--
		return true
	}

	function nextPanel() {
		if (lastPanel) {
			return false
		}

		currentIndex++
		return true
	}

	function indexOfPanel(panel) {
		for (var i in children) {
			if (children[i] === panel) {
				return i
			}
		}

		return undefined
	}

	function indexOfActivePanel(panel) {
		var fallBackIndex
		for (var i in children) {
			var child = children[i]
			if (child.active) {
				return i
			} else if (child === panel) {
				fallBackIndex = i
			}
		}

		return fallBackIndex
	}

	function showPanel(panel) {
		var index = indexOfPanel(panel)
		if (index !== undefined) {
			currentIndex = index
			return true
		}

		return false
	}

	signal hidden()

	onVisibleChanged: {
		if (!visible) {
			hidden()
		}
	}

	onCurrentPanelChanged: {
		if (internal.previousPanel) {
			internal.previousPanel.shown = false
			internal.previousIndex = indexOfPanel(internal.previousPanel)
		} else {
			internal.previousIndex = -1
		}

		if (currentPanel) {
			currentPanel.shown = true
		}

		internal.previousPanel = currentPanel
	}

	QtObject {
		id: internal

		property Item previousPanel
		property int previousIndex: -1
	}
}