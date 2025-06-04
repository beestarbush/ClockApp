pragma Singleton
import QtQuick

import Components
import Bee

Item {
	id: backend

	readonly property var interfaceNames: [
		"FunkyClass"
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

	readonly property int screenRotation: FunkyClass.rotationDegrees
	function greetings() {
		FunkyClass.getGreeting()
	}
}