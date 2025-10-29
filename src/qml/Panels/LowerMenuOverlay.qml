import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(emptyDialog)

    signal close()
    signal dialWheelValueUpdated(int newValue)

    function showVersion() {
        dialogOverlay.showPanel(versionDialog)
    }

    function showSetupWizard() {
        dialogOverlay.showPanel(setupWizardDialog)
    }

    function showScreenBrightnessConfiguration() {
        dialogOverlay.showPanel(screenBrightnessDialog)
    }

    function showBackgroundOpacityConfiguration() {
        dialogOverlay.showPanel(backgroundOpacityDialog)
    }

    function showMediaSelection(index) {
        mediaSelectionDialog.setIndex(index)
        dialogOverlay.showPanel(mediaSelectionDialog)
    }

    function showColorSelection(clockPointerIndex) {
        colorSelectionDialog.setClockPointerIndex(clockPointerIndex)
        dialogOverlay.showPanel(colorSelectionDialog)
    }

    function showNotifications() {
        dialogOverlay.showPanel(notificationDialog)
    }

    function showDialWheel(minimumValue, maximumValue, stepSize, startValue)
    {
        dialWheel.minimumValue = minimumValue
        dialWheel.maximumValue = maximumValue
        dialWheel.stepSize = stepSize
        dialWheel.value = startValue
        dialogOverlay.showPanel(dialWheelDialog)
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
            id: versionValue

		    width: parent.width - Value.defaultMargin
            font.bold: true
            font.pixelSize: Value.largeTextSize
            anchors.centerIn: parent
            text: Backend.version.tag

            wrapMode: Text.Wrap
		    horizontalAlignment: Text.AlignHCenter
		    verticalAlignment: Text.AlignVCenter

            color: Color.lightGray
        }

        Timer {
            id: toggleTextTimer

            interval: 5000 // 5 seconds
            running: versionDialog.visible
            repeat: true

            property bool showVersion: true

            onTriggered: {
                if (showVersion) {
                    versionValue.text = Backend.version.tag
                    showVersion = false
                } else {
                    versionValue.text = BeeBackend.Services.remoteApi.deviceId
                    showVersion = true
                }
            }
        }
    }

    MenuDialog {
        id: setupWizardDialog

        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onPressed: setupWizardLongPressTimer.start()
            onReleased: setupWizardLongPressTimer.stop()
            onCanceled: setupWizardLongPressTimer.stop()
        }

        Timer {
            id: setupWizardLongPressTimer

            interval: 2000
            running: false
            repeat: false
            onTriggered: {
                BeeBackend.Applications.setup.resetSetup()
                dialogOverlay.closePanels()
            }
        }

        Text {
            id: setupWizardTextValue

		    width: parent.width - Value.defaultMargin
            font.pixelSize: Value.defaultTextSize
            anchors.centerIn: parent

            wrapMode: Text.Wrap
		    horizontalAlignment: Text.AlignHCenter
		    verticalAlignment: Text.AlignVCenter

            text: "Press and hold to enter setup wizard."

            color: Color.lightGray
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
        id: mediaSelectionDialog

        anchors.fill: parent
        anchors.centerIn: parent

        property int selectedIndex: 0

        function setIndex(index) {
            mediaSelectionDialog.selectedIndex = index
            if (index == 0) {
                carousel.selectMediaByName(BeeBackend.Applications.clock.background)
            } else if (index == 1) {
                carousel.selectMediaByName(BeeBackend.Applications.marriedTimer.background)
            } else if (index == 2) {
                carousel.selectMediaByName(BeeBackend.Applications.kuikenTimer.background)
            } else if (index == 3) {
                carousel.selectMediaByName(BeeBackend.Applications.countdownTimer.background)
            }
            
        }

        MediaCarousel {
            id: carousel

            anchors.fill: parent
            anchors.centerIn: parent

            onMediaSelected: (mediaName) => {
                console.log("DBG: onMediaSelected: " + mediaName + " index: " + mediaSelectionDialog.selectedIndex)
                if (mediaSelectionDialog.selectedIndex == 0) {
                    BeeBackend.Applications.clock.background = mediaName
                } else if (mediaSelectionDialog.selectedIndex == 1) {
                    BeeBackend.Applications.marriedTimer.background = mediaName
                } else if (mediaSelectionDialog.selectedIndex == 2) {
                    BeeBackend.Applications.kuikenTimer.background = mediaName
                } else if (mediaSelectionDialog.selectedIndex == 3) {
                    BeeBackend.Applications.countdownTimer.background = mediaName
                }
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

    MenuDialog {
        id: notificationDialog

        anchors.fill: parent
        anchors.centerIn: parent

        NotificationCarousel {
            anchors.fill: parent
            anchors.centerIn: parent
            model: BeeBackend.Services.notificationManager
        }
    }

    MenuDialog {
        id: dialWheelDialog

        anchors.fill: parent
        anchors.centerIn: parent

        DialWheel {
            id: dialWheel

            anchors.fill: parent
            anchors.centerIn: parent

            minimumValue: 1
            maximumValue: 31
            stepSize: 1
            value: 1

            onValueChanged: dialogOverlay.dialWheelValueUpdated(dialWheel.value)
        }
    }
}