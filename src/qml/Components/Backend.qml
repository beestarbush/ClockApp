pragma Singleton
import QtQuick

import Components
import Bee as Bee

Item {
	id: backend

	readonly property ScreenQmlObject screen: ScreenQmlObject {}
	readonly property VersionQmlObject version: VersionQmlObject {}
	readonly property ClockQmlObject clock: ClockQmlObject {}
	readonly property KuikenQmlObject kuikenTimer: KuikenQmlObject {}
	readonly property MarriedQmlObject marriedTimer: MarriedQmlObject {}
	readonly property DebugQmlObject debugging: DebugQmlObject {}

	// Animation management properties
	property var availableAnimations: Bee.AnimationManager.availableAnimations

	component ScreenQmlObject: QtObject {
		readonly property QtObject screen: Bee.Screen

		readonly property int brightness: Bee.Screen.brightness

		function setBrightness(brightness) {
			Bee.Screen.brightness = brightness
		}
	}

	component VersionQmlObject: QtObject {
		readonly property string tag: Bee.Version.tag
	}

	component ClockQmlObject : QtObject {
		property bool enabled: true
		property real backgroundOpacity: 0.5
		property string backgroundAnimation: Bee.Settings.clockBackgroundAnimation
		property color hourColor: Bee.Settings.clockHourColor
		property color minuteColor: Bee.Settings.clockMinuteColor
		property color secondColor: Bee.Settings.clockSecondColor
		property color pendulumBobColor: Bee.Settings.clockPendulumColor
		property color pendulumRodColor: Color.black

		function setSelectedAnimation(animationName) {
			Bee.Settings.clockBackgroundAnimation = animationName
		}

		function setHourColor(color) {
			Bee.Settings.clockHourColor = color
		}

		function setMinuteColor(color) {
			Bee.Settings.clockMinuteColor = color
		}

		function setSecondColor(color) {
			Bee.Settings.clockSecondColor = color
		}

		function setPendulumColor(color) {
			Bee.Settings.clockPendulumColor = color
		}
	}

	component MarriedQmlObject : QtObject {
		property bool enabled: Bee.Settings.marriedTimerEnabled
		property real backgroundOpacity: Bee.Settings.marriedTimerBackgroundOpacity
		property string backgroundAnimation: Bee.Settings.marriedTimerBackgroundAnimation

		function setEnabled(enable) {
			Bee.Settings.marriedTimerEnabled = enable
		}
	}

	component KuikenQmlObject : QtObject {
		property bool enabled: Bee.Settings.kuikenTimerEnabled

		function setEnabled(enable) {
			Bee.Settings.kuikenTimerEnabled = enable
		}
	}

	component DebugQmlObject : QtObject {
		property bool panelEnabled: false
	}
}