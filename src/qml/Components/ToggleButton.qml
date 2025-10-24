import QtQuick

import Components

Item {
	id: toggleButton

	enum Size {
		Small,
		Large
	}

	property int size: ToggleButton.Size.Large

	property bool enabled: true
	property QtObject target
	property string property
	property var activator

	signal valueChanged(bool previousValue, bool newValue)
	signal clickedWhileDisabled()

	implicitHeight: (size === ToggleButton.Size.Large) ? Value.largeToggleButtonHeight : Value.smallToggleButtonHeight
	implicitWidth: height * 1.8

	MouseArea {
		id: mouseArea

		anchors.fill: parent

		onClicked: {
			if (toggleButton.enabled) {
				if (activator) {
					activator()
				} else {
					changeValueTimeoutTracker.start()
					var previousValue = target[property]
					target[property] = !previousValue
					valueChanged(previousValue, !previousValue)
				}
			} else {
				toggleButton.clickedWhileDisabled()
			}
		}
	}

	Timer {
		id: changeValueTimeoutTracker

		interval: Value.changeValueTimeout

		onTriggered: {
			console.warn("Timeout encountered when setting property", property, "of", target)
		}
	}

	Circle {
		id: circle

		property bool toggleState: target ? target[property] : false
		property bool targetToggleState: (toggleButton.enabled
										  && (mouseArea.pressed || changeValueTimeoutTracker.running)) ? !toggleState
																									   : toggleState
		onToggleStateChanged: {
			changeValueTimeoutTracker.stop()
		}

		anchors.fill: parent

		color: toggleButton.enabled ? (targetToggleState ? Color.green1 : Color.darkGray)
									: (targetToggleState ? Color.disabledToggleActive
														 : Color.disabledToggleInactive)

		Circle {
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.margins: Value.smallMargin
			width: height
			color: Color.white
			x: Value.smallMargin + (parent.targetToggleState ? (parent.width - width - 2 * Value.smallMargin) : 0)

			BehaviorWhenVisible on x { NumberAnimation { duration: Value.defaultAnimationDuration } }
		}

		BehaviorWhenVisible on color { ColorAnimation { duration: Value.defaultAnimationDuration } }
	}
}