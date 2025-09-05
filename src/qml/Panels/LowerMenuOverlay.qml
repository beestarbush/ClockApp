import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

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
            color: BeeBackend.Applications.kuikenTimer.enabled ? Color.green1 : Color.red

            onClicked: {
                if (BeeBackend.Applications.kuikenTimer.enabled) {
                    BeeBackend.Applications.kuikenTimer.enabled = false
                }
                else {
                    BeeBackend.Applications.kuikenTimer.enabled = true
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
            color: BeeBackend.Applications.marriedTimer.enabled ? Color.green1 : Color.red

            onClicked: {
                if (BeeBackend.Applications.marriedTimer.enabled) {
                    BeeBackend.Applications.marriedTimer.enabled = false
                }
                else {
                    BeeBackend.Applications.marriedTimer.enabled = true
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
            property real value: BeeBackend.Applications.clock.backgroundOpacity

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
                    BeeBackend.Applications.clock.backgroundOpacity = opacityButton.value
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
                BeeBackend.Applications.clock.backgroundAnimation = animationName
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
                colorWheel.startColor = BeeBackend.Applications.clock.hourColor
            } else if (clockPointerIndex == 1) {
                colorWheel.startColor = BeeBackend.Applications.clock.minuteColor
            } else if (clockPointerIndex == 2) {
                colorWheel.startColor = BeeBackend.Applications.clock.secondColor
            } else if (clockPointerIndex == 3) {
                colorWheel.startColor = BeeBackend.Applications.clock.pendulumBobColor
            }
        }

        ColorWheel {
            id: colorWheel

            anchors.fill: parent
            anchors.centerIn: parent

            onColorSelected: (selectedColor) => {       
                if (colorSelectionDialog.clockPointerIndex == 0) {
                    BeeBackend.Applications.clock.hourColor = selectedColor
                } else if (colorSelectionDialog.clockPointerIndex == 1) {
                    BeeBackend.Applications.clock.minuteColor = selectedColor
                } else if (colorSelectionDialog.clockPointerIndex == 2) {
                    BeeBackend.Applications.clock.secondColor = selectedColor
                } else if (colorSelectionDialog.clockPointerIndex == 3) {
                    BeeBackend.Applications.clock.pendulumBobColor = selectedColor
                }
            }
        }
    }
}