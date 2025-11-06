import QtQuick
import QtQuick.Controls

import Components
import Bee as Backend

PanelContainer {
    id: overlay

    property var menu: Backend.Applications.menu
    property var setup: Backend.Applications.setup

    currentIndex: {
        // Setup dialogs take priority only when setup is not complete
        if (setup && !setup.setupComplete) {
            if (setup.dialWheel && setup.dialWheel.visible) {
                return indexOfPanel(dialWheelDialog)
            }
            if (setup.mediaSelection && setup.mediaSelection.visible) {
                return indexOfPanel(mediaSelectionDialog)
            }
        }
        
        // Otherwise show menu dialogs
        switch(menu ? menu.dialog : 0) {
            case Backend.MenuEnums.Version: return indexOfPanel(versionDialog)
            case Backend.MenuEnums.SetupWizard: return indexOfPanel(setupWizardDialog)
            case Backend.MenuEnums.ScreenBrightness: return indexOfPanel(screenBrightnessDialog)
            case Backend.MenuEnums.BackgroundOpacity: return indexOfPanel(backgroundOpacityDialog)
            case Backend.MenuEnums.MediaSelection: return indexOfPanel(mediaSelectionDialog)
            case Backend.MenuEnums.ColorSelection: return indexOfPanel(colorSelectionDialog)
            case Backend.MenuEnums.Notifications: return indexOfPanel(notificationDialog)
            case Backend.MenuEnums.DialWheel: return indexOfPanel(dialWheelDialog)
            default: return indexOfPanel(emptyDialog)
        }
    }

    signal close()
    signal dialWheelValueUpdated(int newValue)

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
            onTriggered: Backend.Applications.debug.panelEnabled = true
        }

        Text {
            id: versionValue
            width: parent.width - Value.defaultMargin
            font.bold: true
            font.pixelSize: Value.largeTextSize
            anchors.centerIn: parent
            text: Backend.Services.version.tag
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Color.lightGray
        }

        Timer {
            id: toggleTextTimer
            interval: 5000
            running: versionDialog.visible
            repeat: true
            property bool showVersion: true

            onTriggered: {
                if (showVersion) {
                    versionValue.text = Backend.Services.version.tag
                    showVersion = false
                } else {
                    versionValue.text = Backend.Services.remoteApi.deviceId
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
                Backend.Applications.setup.reset()
                menu.closeDialog()
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
            property real value: Backend.Drivers.screen.brightness

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

                    Backend.Drivers.screen.brightness = brightnessButton.value
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
            property real value: Backend.Applications.clock.configuration.backgroundOpacity

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
                    Backend.Applications.clock.configuration.backgroundOpacity = opacityButton.value
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

        MediaCarousel {
            id: carousel
            anchors.fill: parent
            anchors.centerIn: parent
            media: Backend.Services.media.model

            // Function to update selected media based on current mode
            function updateSelectedMedia() {
                // Check if we're in setup mode or menu mode
                if (!setup.setupComplete && setup.mediaSelection.visible) {
                    var setupTarget = setup.mediaSelection.target
                    if (setupTarget === Backend.SetupEnums.MarriedTarget) {
                        carousel.selectMediaByName(Backend.Applications.marriedTimer.configuration.background)
                    } else if (setupTarget === Backend.SetupEnums.KuikenTarget) {
                        carousel.selectMediaByName(Backend.Applications.kuikenTimer.configuration.background)
                    } else if (setupTarget === Backend.SetupEnums.CountdownTarget) {
                        carousel.selectMediaByName(Backend.Applications.countdownTimer.configuration.background)
                    }
                } else {
                    var param = menu.dialogParam
                    if (param === Backend.MenuEnums.ClockBackground) {
                        carousel.selectMediaByName(Backend.Applications.clock.configuration.background)
                    } else if (param === Backend.MenuEnums.Married) {
                        carousel.selectMediaByName(Backend.Applications.marriedTimer.configuration.background)
                    } else if (param === Backend.MenuEnums.Kuiken) {
                        carousel.selectMediaByName(Backend.Applications.kuikenTimer.configuration.background)
                    } else if (param === Backend.MenuEnums.Countdown) {
                        carousel.selectMediaByName(Backend.Applications.countdownTimer.configuration.background)
                    }
                }
            }

            Component.onCompleted: updateSelectedMedia()

            // React to dialog parameter changes
            Connections {
                target: menu
                function onDialogParamChanged() {
                    carousel.updateSelectedMedia()
                }
            }

            // React to setup target changes
            Connections {
                target: setup
                function onMediaSelectionChanged() {
                    carousel.updateSelectedMedia()
                }
            }

            onMediaSelected: (mediaName) => {
                // Check if we're in setup mode or menu mode
                if (!setup.setupComplete && setup.mediaSelection.visible) {
                    setup.selectMedia(setup.mediaSelection.target, mediaName)
                } else {
                    menu.setBackground(menu.dialogParam, mediaName)
                }
            }
        }
    }

    MenuDialog {
        id: colorSelectionDialog
        anchors.fill: parent
        anchors.centerIn: parent

        ColorWheel {
            id: colorWheel
            anchors.fill: parent
            anchors.centerIn: parent

            startColor: {
                var param = menu ? menu.dialogParam : null
                if (!param) return "white"
                if (param === Backend.MenuEnums.Hours) {
                    return Backend.Applications.clock.configuration.hourColor
                } else if (param === Backend.MenuEnums.Minutes) {
                    return Backend.Applications.clock.configuration.minuteColor
                } else if (param === Backend.MenuEnums.Seconds) {
                    return Backend.Applications.clock.configuration.secondColor
                } else if (param === Backend.MenuEnums.Pendulum) {
                    return Backend.Applications.clock.configuration.pendulumBobColor
                }
                return "white"
            }

            onColorSelected: (selectedColor) => {
                menu.setColor(menu.dialogParam, selectedColor)
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
            model: Backend.Services.notification.model
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
            minimumValue: (setup && !setup.setupComplete && setup.dialWheel && setup.dialWheel.visible) ? setup.dialWheel.min : 1
            maximumValue: (setup && !setup.setupComplete && setup.dialWheel && setup.dialWheel.visible) ? setup.dialWheel.max : 31
            stepSize: (setup && !setup.setupComplete && setup.dialWheel && setup.dialWheel.visible) ? setup.dialWheel.step : 1
            value: (setup && !setup.setupComplete && setup.dialWheel && setup.dialWheel.visible) ? setup.dialWheel.value : 1

            onValueChanged: {
                if (setup && !setup.setupComplete && setup.dialWheel && setup.dialWheel.visible) {
                    // In setup mode, just emit signal for SetupPanel to handle
                    dialWheelValueUpdated(dialWheel.value)
                } else {
                    // In menu mode, update menu
                    menu.dialWheelValueChanged(dialWheel.value)
                }
            }
        }
    }
}