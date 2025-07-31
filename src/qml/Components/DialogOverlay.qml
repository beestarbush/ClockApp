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
                text: "S"
                onClicked: menuDialog.openSettingsDialog()
            }

            RoundButton {
                color: Color.green1
                text: "I"
                onClicked: menuDialog.openInfoDialog()
            }

            RoundButton {
                color: Color.green1
                text: "X"
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
                text: "Version:\n" + Backend.tag
            }

            RoundButton {
                color: Color.green1
                text: "X"

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
                id: opacityButton

                property real minValue: 0.0
                property real maxValue: 1.0
                property real step: 0.01
                property bool incrementMode: true // true: increment, false: decrement

                // The value you want to control
                property real value: Backend.clockBackgroundAnimationOpacity

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
                    id: holdTimer
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
                        Backend.clockBackgroundAnimationOpacity = opacityButton.value
                    }
                }

                onPressed: holdTimer.start()
                onReleased: {
                    holdTimer.stop()
                    // Toggle increment/decrement mode
                    incrementMode = !incrementMode
                }
            }

            RoundButton {
                color: Backend.kuikenTimerEnabled ? Color.green1 : Color.red
                text: "Kuiken"

                onClicked: {
                    if (Backend.kuikenTimerEnabled) {
                        Backend.kuikenTimerEnabled = false
                    }
                    else {
                        Backend.kuikenTimerEnabled = true
                    }
                }
            }

            RoundButton {
                color: Backend.marriedTimerEnabled ? Color.green1 : Color.red
                text: "Wedding"

                onClicked: {
                    if (Backend.marriedTimerEnabled) {
                        Backend.marriedTimerEnabled = false
                    }
                    else {
                        Backend.marriedTimerEnabled = true
                    }
                }
            }

            RoundButton {
                color: Color.green1
                text: "X"

                onClicked: settingsDialog.closeDialog()
            }
        }

        onCloseDialog: panelContainer.showPanel(menuDialog)
    }
}