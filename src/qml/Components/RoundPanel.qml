import QtQuick

import Components

Panel {
	property alias backgroundColor: circlePanel.color
	Circle {
		id: circlePanel

		anchors.fill: parent
	}
}