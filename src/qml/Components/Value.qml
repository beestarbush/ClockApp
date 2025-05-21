pragma Singleton
import QtQuick

QtObject {
	id: value

	readonly property int hourInMinutes: 60
	readonly property int minuteInSeconds: 60
	readonly property int secondInMilliseconds: 1000
	readonly property int hourInSeconds: hourInMinutes * minuteInSeconds
	readonly property int minuteInMilliseconds: minuteInSeconds * secondInMilliseconds
	readonly property int hourInMilliseconds: hourInSeconds * secondInMilliseconds

	readonly property int xSmallButtonHeight: 40
	readonly property int smallButtonHeight: 60
	readonly property int largeButtonHeight: 80

	readonly property int smallToggleButtonHeight: 44
	readonly property int largeToggleButtonHeight: smallButtonHeight

	property real animationDurationScaleFactor: 1
	readonly property int shortAnimationDuration: 80 * animationDurationScaleFactor
	readonly property int defaultAnimationDuration: 150 * animationDurationScaleFactor

	readonly property int changeValueTimeout: secondInMilliseconds

	readonly property int defaultMargin: 20
	readonly property int largeMargin: 40
	readonly property int smallMargin: 4

	property bool dragAndOvershootBounds: false
	readonly property int boundsBehavior: dragAndOvershootBounds ? Flickable.DragAndOvershootBounds : Flickable.StopAtBounds
	readonly property int flickDeceleration: 4500

	readonly property int smallTextSize: 18
	readonly property int defaultTextSize: 20
	readonly property int largeTextSize: 32
	readonly property int veryLargeTextSize: 56

	function defaultWidthRatio(referenceItem) {
		return referenceItem.width * 0.7
	}
}