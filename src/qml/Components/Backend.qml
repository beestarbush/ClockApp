pragma Singleton
import QtQuick

import Components
import Bee as Bee

Item {
	id: backend

	readonly property var interfaceNames: [
		"Version",
		"Screen"
	]
	function interfacesAvailable() {
		for (var i in interfaceNames) {
			var interfaceName = interfaceNames[i]
			if (!Bee[interfaceName]) {
				console.warn("Interface", interfaceName, "unavailable!")
				return false
			}
		}
		console.info("All interfaces available!")
		return true
	}

	readonly property ScreenQmlObject screen: ScreenQmlObject {}
	readonly property VersionQmlObject version: VersionQmlObject {}

	property real clockBackgroundAnimationOpacity: 0.50
	property bool marriedTimerEnabled: false
	property bool kuikenTimerEnabled: false
	property bool clockEnabled: true
	property bool debugPanelEnabled: false

	property color hourColor: Color.red
	property color minuteColor: Color.blue
	property color secondColor: Color.green1
	property color pendulumBobColor: Color.green1
	property color pendulumRodColor: Color.black

	// Animation management properties
	property var availableAnimations: Bee.AnimationManager.availableAnimations
	property string clockBackgroundAnimation: "test.gif"

	function setSelectedAnimation(animationName) {
		clockBackgroundAnimation = animationName
	}

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
}