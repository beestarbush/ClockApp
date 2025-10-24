pragma Singleton
import QtQuick

import Components
import Bee as Bee

Item {
	id: backend

	readonly property ScreenQmlObject screen: ScreenQmlObject {}
	readonly property VersionQmlObject version: VersionQmlObject {}
	readonly property DebugQmlObject debugging: DebugQmlObject {}

	// Media management properties
	property var availableMedia: Bee.Services.mediaManager.availableMedia

	component ScreenQmlObject: QtObject {
		readonly property QtObject screen: Bee.HAL.screen

		readonly property int brightness: Bee.HAL.screen.brightness

		function setBrightness(brightness) {
			Bee.HAL.screen.brightness = brightness
		}
	}

	component VersionQmlObject: QtObject {
		readonly property string tag: Bee.Services.version.tag
	}

	component DebugQmlObject : QtObject {
		property bool panelEnabled: false
	}
}