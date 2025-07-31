pragma Singleton
import QtQuick

import Components
import Bee

Item {
	id: backend

	readonly property var interfaceNames: [
		"Version"
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

	readonly property string tag: Version.tag
	property real clockBackgroundAnimationOpacity: 0.75
	property bool marriedTimerEnabled: false
	property bool kuikenTimerEnabled: false
	property bool clockEnabled: true
}