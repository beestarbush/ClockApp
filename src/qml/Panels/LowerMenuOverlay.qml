import QtQuick
import QtQuick.Controls

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(emptyDialog)

    signal close()

    function showVersion(value) {
        versionDialog.text = value
        dialogOverlay.showPanel(versionDialog)
    }

    function showKuikenTimerConfiguration() {
        dialogOverlay.showPanel(kuikenTimerDialog)
    }

    function showMarriedTimerConfiguration() {
        dialogOverlay.showPanel(marriedTimerDialog)
    }

    function showScreenBrightnessConfiguration() {
        dialogOverlay.showPanel(screenBrightnessDialog)
    }

    function showBackgroundOpacityConfiguration() {
        dialogOverlay.showPanel(backgroundOpacityDialog)
    }

    function showAnimationSelection() {
        dialogOverlay.showPanel(animationSelectionDialog)
    }

    function showColorSelection(clockPointerIndex) {
        colorSelectionDialog.setClockPointerIndex(clockPointerIndex)
        dialogOverlay.showPanel(colorSelectionDialog)
    }

    function closePanels() {
        dialogOverlay.showPanel(emptyDialog)
    }

    MenuDialog {
        id: emptyDialog

        anchors.fill: parent
    }

    MenuDialog {
        id: versionDialog

        property alias text: textValue.text
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onPressed: longPressTimer.start()
            onReleased: longPressTimer.stop()
            onCanceled: longPressTimer.stop()
        }

        Timer {
            id: longPressTimer

            interval: 2000
            running: false
            repeat: false
            onTriggered: Backend.debugging.panelEnabled = true
        }

        Text {
            id: textValue

		    width: parent.width - Value.defaultMargin
            font.bold: true
            font.pixelSize: Value.largeTextSize
            anchors.centerIn: parent

            wrapMode: Text.Wrap
		    horizontalAlignment: Text.AlignHCenter
		    verticalAlignment: Text.AlignVCenter

            color: Color.lightGray
        }
    }

    MenuDialog {
        id: kuikenTimerDialog

        anchors.fill: parent

        RoundButton {
		    anchors.fill: parent
            anchors.centerIn: parent           
            color: Backend.kuikenTimer.enabled ? Color.green1 : Color.red

            onClicked: {
                if (Backend.kuikenTimer.enabled) {
                    Backend.kuikenTimer.setEnabled(false)
                }
                else {
                    Backend.kuikenTimer.setEnabled(true)
                }
            }
        }     
    }

    MenuDialog {
        id: marriedTimerDialog

        anchors.fill: parent

        RoundButton {
		    anchors.fill: parent
            anchors.centerIn: parent           
            color: Backend.marriedTimer.enabled ? Color.green1 : Color.red

            onClicked: {
                if (Backend.marriedTimer.enabled) {
                    Backend.marriedTimer.setEnabled(false)
                }
                else {
                    Backend.marriedTimer.setEnabled(true)
                }
            }
        }     
    }

    MenuDialog {
        id: screenBrightnessDialog

        anchors.fill: parent

        RoundButton {
            id: brightnessButton

            anchors.fill: parent
            anchors.centerIn: parent

            property real minValue: 0
            property real maxValue: 100
            property real step: 1
            property bool incrementMode: true // true: increment, false: decrement

            // The value you want to control
            property real value: Backend.screen.brightness

            // Interpolate between Color.gray and Color.green1
            function lerpColor(a, b, t) {
                return Qt.rgba(
                    a.r + (b.r - a.r) * t,
                    a.g + (b.g - a.g) * t,
                    a.b + (b.b - a.b) * t,
                    a.a + (b.a - a.a) * t
                )
            }
            color: lerpColor(Color.gray, Color.green1, value / 100)

            text: Math.round(value) + "%"

            Timer {
                id: brightnessHoldTimer
                interval: 60
                repeat: true
                running: false
                onTriggered: {
                    if (brightnessButton.incrementMode) {
                        if (brightnessButton.value < brightnessButton.maxValue) {
                            brightnessButton.value = Math.min(brightnessButton.value + brightnessButton.step, brightnessButton.maxValue)
                        }
                    } else {
                        if (brightnessButton.value > brightnessButton.minValue) {
                            brightnessButton.value = Math.max(brightnessButton.value - brightnessButton.step, brightnessButton.minValue)
                        }
                    }

                    Backend.screen.setBrightness(brightnessButton.value)
                }
            }

            onPressed: brightnessHoldTimer.start()
            onReleased: {
                brightnessHoldTimer.stop()
                // Toggle increment/decrement mode
                incrementMode = !incrementMode
            }
        }
    }

    MenuDialog {
        id: backgroundOpacityDialog

        anchors.fill: parent

        RoundButton {
            id: opacityButton

            anchors.fill: parent
            anchors.centerIn: parent

            property real minValue: 0.0
            property real maxValue: 1.0
            property real step: 0.01
            property bool incrementMode: true // true: increment, false: decrement

            // The value you want to control
            property real value: Backend.clock.backgroundOpacity

            // Interpolate between Color.gray and Color.green1
            function lerpColor(a, b, t) {
                return Qt.rgba(
                    a.r + (b.r - a.r) * t,
                    a.g + (b.g - a.g) * t,
                    a.b + (b.b - a.b) * t,
                    a.a + (b.a - a.a) * t
                )
            }
            color: lerpColor(Color.gray, Color.green1, value)

            text: Math.round(value * 100) + "%"

            Timer {
                id: opacityHoldTimer
                interval: 60
                repeat: true
                running: false
                onTriggered: {
                    if (opacityButton.incrementMode) {
                        if (opacityButton.value < opacityButton.maxValue) {
                            opacityButton.value = Math.min(opacityButton.value + opacityButton.step, opacityButton.maxValue)
                        }
                    } else {
                        if (opacityButton.value > opacityButton.minValue) {
                            opacityButton.value = Math.max(opacityButton.value - opacityButton.step, opacityButton.minValue)
                        }
                    }
                    Backend.clock.backgroundOpacity = opacityButton.value
                }
            }

            onPressed: opacityHoldTimer.start()
            onReleased: {
                opacityHoldTimer.stop()
                // Toggle increment/decrement mode
                incrementMode = !incrementMode
            }
        }
    }

    MenuDialog {
        id: animationSelectionDialog

        anchors.fill: parent
        anchors.centerIn: parent

        AnimationCarousel {
            anchors.fill: parent
            anchors.centerIn: parent

            onAnimationSelected: (animationName) => {
                Backend.clock.setSelectedAnimation(animationName)
            }
        }
    }

    MenuDialog {
        id: colorSelectionDialog

        anchors.fill: parent
        anchors.centerIn: parent

        property int clockPointerIndex: 0

        function setClockPointerIndex(clockPointerIndex) {
            colorSelectionDialog.clockPointerIndex = clockPointerIndex
            if (clockPointerIndex == 0) {
                colorWheel.startColor = Backend.clock.hourColor
            } else if (clockPointerIndex == 1) {
                colorWheel.startColor = Backend.clock.minuteColor
            } else if (clockPointerIndex == 2) {
                colorWheel.startColor = Backend.clock.secondColor
            } else if (clockPointerIndex == 3) {
                colorWheel.startColor = Backend.clock.pendulumBobColor
            }
        }

        ColorWheel {
            id: colorWheel

            anchors.fill: parent
            anchors.centerIn: parent

            onColorSelected: (selectedColor) => {       
                if (colorSelectionDialog.clockPointerIndex == 0) {
                    Backend.clock.setHourColor(selectedColor)
                } else if (colorSelectionDialog.clockPointerIndex == 1) {
                    Backend.clock.setMinuteColor(selectedColor)
                } else if (colorSelectionDialog.clockPointerIndex == 2) {
                    Backend.clock.setSecondColor(selectedColor)
                } else if (colorSelectionDialog.clockPointerIndex == 3) {
                    Backend.clock.setPendulumColor(selectedColor)
                }
            }
        }
    }
}