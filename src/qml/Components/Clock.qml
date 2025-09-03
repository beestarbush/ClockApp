import QtQuick

import Bee as Bee
import Components

Circle {
	id: clock

	color: Color.transparent

	property int hours
	property int minutes
	property int seconds

	readonly property int hourHandWidthScale: 25
	readonly property int minuteHandWidthScale: 30
	readonly property int secondHandWidthScale: 35
	readonly property int centerPointScale: 15

	property alias hourColor: hourPointer.color
	property alias minuteColor: minutePointer.color
	property alias secondColor: secondPointer.color

	function timeChanged() {
		var now = Bee.Services.dateTime.localTime

		var parts = now.split(":");

		hours = parseInt(parts[0]);
		minutes = parseInt(parts[1]);
		seconds = parseInt(parts[2]);
	}

	Timer {
		interval: 100
		repeat: true
		running: true
		onTriggered: timeChanged()
	}

	Rectangle {
		id: minutePointer
		width: parent.width / minuteHandWidthScale
		height: clock.height / 2.5
		anchors.horizontalCenter: clock.horizontalCenter
		y: clock.y + clock.height / 2 - height

		color: Color.blue
		radius: 3

		transformOrigin: Item.Bottom
		transform: Rotation {
			id: minuteRotation

			origin.x: minutePointer.width / 2
			origin.y: minutePointer.height
			angle: (clock.minutes * 6)
			
			Behavior on angle {
				SpringAnimation {
					spring: 2
					damping: 0.2
					modulus: 360
				}
			}
		}
	}

	Rectangle {
		id: hourPointer

		width: parent.width / hourHandWidthScale
		height: clock.height / 4
		anchors.horizontalCenter: clock.horizontalCenter
		y: clock.y + clock.height / 2 - height

		color: Color.red
		radius: 3
		
		transform: Rotation {
			id: hourRotation

			origin.x: hourPointer.width / 2
			origin.y: hourPointer.height
			angle: (clock.hours * 30) + (clock.minutes * 0.5)
			
			Behavior on angle {
				SpringAnimation {
					spring: 2
					damping: 0.2
					modulus: 360
				}
			}
		}
	}

	Rectangle {
		id: secondPointer
		width: parent.width / secondHandWidthScale
		height: clock.height / 2
		anchors.horizontalCenter: clock.horizontalCenter
		y: clock.y + clock.height / 2 - height

		color: Color.green1
		radius: 3

		transform: Rotation {
			id: secondRotation

			origin.x: secondPointer.width / 2
			origin.y: secondPointer.height
			angle: (clock.seconds * 6)
			
			Behavior on angle {
				SpringAnimation {
					spring: 2
					damping: 0.2
					modulus: 360
				}
			}
		}
	}

	Circle {
		id: centerPoint
		width: parent.width / centerPointScale
		height: width
		anchors.centerIn: parent

		color: Color.black
	}
}
