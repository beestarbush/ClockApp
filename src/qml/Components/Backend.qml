pragma Singleton
import QtQuick

import Components
import Bee as Bee

Item {
	id: backend

	readonly property ScreenQmlObject screen: ScreenQmlObject {}
	readonly property VersionQmlObject version: VersionQmlObject {}
	readonly property ClockQmlObject clock: ClockQmlObject {}
	readonly property TimeSinceQmlObject kuikenTimer: TimeSinceQmlObject {}
	readonly property TimeSinceQmlObject marriedTimer: TimeSinceQmlObject {}
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
		property string backgroundAnimation: "test.gif"
		property color hourColor: Color.red
		property color minuteColor: Color.blue
		property color secondColor: Color.green1
		property color pendulumBobColor: Color.green1
		property color pendulumRodColor: Color.black

		function setSelectedAnimation(animationName) {
			backgroundAnimation = animationName
		}
	}

	component TimeSinceQmlObject : QtObject {
		property bool enabled: false
	}

	component DebugQmlObject : QtObject {
		property bool panelEnabled: false
	}
}