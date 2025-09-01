import QtQuick
import QtQuick.Controls

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(menuDialog)

    signal close()

    MenuDialog {
        id: menuDialog

        //title: "Main menu"
        signal openSettingsDialog()
        signal openInfoDialog()

        RoundLayout {
            anchors.fill: parent

            RoundButton {
                color: Color.green1
                text: "Settings"
                onClicked: menuDialog.openSettingsDialog()
            }

            RoundButton {
                color: Color.green1
                text: "Info"
                onClicked: menuDialog.openInfoDialog()
            }

            RoundButton {
                color: Color.green1
                text: "Close"
                onClicked: dialogOverlay.close()
            }
        }

        onOpenSettingsDialog: dialogOverlay.showPanel(settingsDialog)
        onOpenInfoDialog: dialogOverlay.showPanel(infoDialog)
    }

    MenuDialog {
        id: infoDialog

        signal closeDialog()

        RoundLayout {
            anchors.fill: parent

            RoundButton {
                color: Color.green1
                text: "Version:\n" + Backend.version.tag
            }

            RoundButton {
                color: Color.green1
                text: "Close"

                onClicked: infoDialog.closeDialog()
            }
        }

        onCloseDialog: dialogOverlay.showPanel(menuDialog)
    }

    MenuDialog {
        id: settingsDialog

        signal closeDialog()

        RoundLayout {
            anchors.fill: parent

            RoundButton {
                id: brightnessButton

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

                text: "Brightness: " + Math.round(value) + "%"

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

            RoundButton {
                id: opacityButton

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

                text: "Background: " + Math.round(value * 100) + "%"

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

            RoundButton {
                color: Backend.kuikenTimer.enabled ? Color.green1 : Color.red
                text: "Kuiken timer"

                onClicked: {
                    if (Backend.kuikenTimer.enabled) {
                        Backend.kuikenTimer.enabled = false
                    }
                    else {
                        Backend.kuikenTimer.enabled = true
                    }
                }
            }

            RoundButton {
                color: Backend.marriedTimer.enabled ? Color.green1 : Color.red
                text: "Marry timer"

                onClicked: {
                    if (Backend.marriedTimer.enabled) {
                        Backend.marriedTimer.enabled = false
                    }
                    else {
                        Backend.marriedTimer.enabled = true
                    }
                }
            }

            RoundButton {
                color: Color.green1
                text: "Close"

                onClicked: settingsDialog.closeDialog()
            }
        }

        onCloseDialog: panelContainer.showPanel(menuDialog)
    }
}