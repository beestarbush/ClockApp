pragma Singleton
import QtQuick

import Components
import Bee

Item {
	id: backend

	readonly property var interfaceNames: [
		"FunkyClass"
	]
	readonly property bool interfacesAvailable: {
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

	//readonly property bool testBool: Bee.FunkyClass.qTestBool
	function greetings() {
		FunkyClass.getGreeting()
	}
}