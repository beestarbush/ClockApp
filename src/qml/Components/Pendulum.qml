import QtQuick

import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
	id: pendulum

	readonly property int pendulumAngle: 10
	property alias rodColor: rod.color
	property alias bobColor: bob.color
	property bool active: false

	onActiveChanged: {
		if (active) {
			swingAnimation.start()
		}
		else {
			swingAnimation.stop()
		}
	}

	Rectangle {
		id: rod
		width: pendulum.width / 10
		height: pendulum.height / 2
		radius: 25
		color: Color.black
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter

		transform: Rotation {
			id: swing
			origin.x: rod.width / 2
			origin.y: 0
			angle: 0
		}

		Circle {
			id: bob
			width: pendulum.height / 1.25
			height: width
			color: Color.green1
			//anchors.top: rod.verticalCenter
			anchors.top: rod.bottom
			anchors.topMargin: -(height / 2)
			anchors.horizontalCenter: parent.horizontalCenter
		}

		SequentialAnimation {
			id: swingAnimation
			loops: Animation.Infinite

			NumberAnimation {
				target: swing
				property: "angle"
				from: pendulumAngle
				to: -pendulumAngle
				duration: 1000 // 1 second for the full swing
				easing.type: Easing.InOutSine // Smooth easing for natural swing
			}

			NumberAnimation {
				target: swing
				property: "angle"
				from: -pendulumAngle
				to: pendulumAngle
				duration: 1000 // 1 second for the return swing
				easing.type: Easing.InOutSine // Smooth easing for natural swing
			}
		}
	}
}
