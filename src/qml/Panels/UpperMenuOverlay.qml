import QtQuick
import QtQuick.Controls

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(mainDialog)

    property Item lowerMenuOverlay

    signal close()

    enum MainMenu {
        Menu,
        Settings,
        Notifications,
        Backgrounds,
        Colors,
        Version
    }

    enum SettingsMenu {
        DisplayBrightness,
        BackgroundOpacity,
        SetupWizard
    }

    enum ColorMenu {
        Hours,
        Minutes,
        Seconds,
        Pendulum
    }

    enum TimerMenu {
        Married,
        Kuiken
    }

    MenuDialog {
        id: mainDialog

        backgroundOpacity: 0
        signal closeDialog()

        ListModel {
            id: mainMenuModel

            ListElement { menuId: UpperMenuOverlay.MainMenu.Menu; label: "Menu" }
            ListElement { menuId: UpperMenuOverlay.MainMenu.Settings; label: "Settings" }
            ListElement { menuId: UpperMenuOverlay.MainMenu.Notifications; label: "Notifications" }
            ListElement { menuId: UpperMenuOverlay.MainMenu.Backgrounds; label: "Backgrounds" }
            ListElement { menuId: UpperMenuOverlay.MainMenu.Colors; label: "Colors" }
            ListElement { menuId: UpperMenuOverlay.MainMenu.Version; label: "Version" }
        }

        ListModel {
            id: settingsMenuModel

            ListElement { menuId: UpperMenuOverlay.SettingsMenu.DisplayBrightness; label: "Display brightness" }
            ListElement { menuId: UpperMenuOverlay.SettingsMenu.BackgroundOpacity; label: "Background opacity" }
            ListElement { menuId: UpperMenuOverlay.SettingsMenu.SetupWizard; label: "Setup wizard" }
        }

        ListModel {
            id: colorMenuModel

            ListElement { menuId: UpperMenuOverlay.ColorMenu.Hours; label: "Hours" }
            ListElement { menuId: UpperMenuOverlay.ColorMenu.Minutes; label: "Minutes" }
            ListElement { menuId: UpperMenuOverlay.ColorMenu.Seconds; label: "Seconds" }
            ListElement { menuId: UpperMenuOverlay.ColorMenu.Pendulum; label: "Pendulum" }
        }

        ListModel {
            id: timerModel

            ListElement { menuId: UpperMenuOverlay.TimerMenu.Married; label: "Married" }
            ListElement { menuId: UpperMenuOverlay.TimerMenu.Kuiken; label: "Kuiken" }
        }

        RingMenu {
            id: mainRingMenu
            anchors.centerIn: parent
            anchors.fill: parent          
            model: mainMenuModel

            onItemSelected: (index) => {
                var menuId = mainMenuModel.get(index).menuId
                if (menuId === UpperMenuOverlay.MainMenu.Settings) {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = true
                }
                else if (menuId === UpperMenuOverlay.MainMenu.Notifications) {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = false
                    lowerMenuOverlay.showNotifications()
                }
                else if (menuId === UpperMenuOverlay.MainMenu.Backgrounds) {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = false
                    lowerMenuOverlay.showMediaSelection()
                }
                else if (menuId === UpperMenuOverlay.MainMenu.Colors) {
                    settingsRingMenu.visible = false
                    colorRingMenu.visible = true
                }
                else if (menuId === UpperMenuOverlay.MainMenu.Version)
                {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = false
                    lowerMenuOverlay.showVersion(Backend.version.tag)
                }
                else {
                    settingsRingMenu.visible = false
                    settingsRingMenu.reset()

                    colorRingMenu.visible = false
                    colorRingMenu.reset()

                    lowerMenuOverlay.closePanels()
                }
            }

            RingMenu {
                id: settingsRingMenu

                visible: mainRingMenu.selectedIndex == UpperMenuOverlay.MainMenu.Settings
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: settingsMenuModel

                function evaluateLowerMenuOverlay(index) {
                    var menuId = settingsMenuModel.get(index).menuId
                    if (menuId === UpperMenuOverlay.SettingsMenu.DisplayBrightness) {
                        lowerMenuOverlay.showScreenBrightnessConfiguration()
                    } else if (menuId === UpperMenuOverlay.SettingsMenu.BackgroundOpacity) {
                        lowerMenuOverlay.showBackgroundOpacityConfiguration()
                    } else if (menuId === UpperMenuOverlay.SettingsMenu.SetupWizard) {
                        lowerMenuOverlay.showSetupWizard()
                    }
                    else {
                        lowerMenuOverlay.closePanels()
                    }
                }

                onVisibleChanged: {
                    if (visible) {
                        evaluateLowerMenuOverlay(settingsRingMenu.selectedIndex)
                    }
                }

                onItemSelected: (index) => {
                    evaluateLowerMenuOverlay(index)
                }
            }

            RingMenu {
                id: colorRingMenu

                visible: mainRingMenu.selectedIndex == UpperMenuOverlay.MainMenu.Colors
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: colorMenuModel

                function evaluateLowerMenuOverlay(index) {
                    if (index == 0 ||
                        index == 1 ||
                        index == 2 ||
                        index == 3) {
                        lowerMenuOverlay.showColorSelection(index)
                    } else {
                        lowerMenuOverlay.closePanels()
                    }
                }

                onVisibleChanged: {
                    if (visible) {
                        evaluateLowerMenuOverlay(colorRingMenu.selectedIndex)
                    }
                }

                onItemSelected: (index) => {
                    evaluateLowerMenuOverlay(index)
                }
            }

            Circle {
                anchors.centerIn: parent
                width: settingsRingMenu.visible || colorRingMenu.visible ? settingsRingMenu.width - 200 : mainRingMenu.width - 200
                height: settingsRingMenu.visible || colorRingMenu.visible ? settingsRingMenu.height - 200 : mainRingMenu.height - 200
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        dialogOverlay.close()
                        lowerMenuOverlay.closePanels()
                        mainRingMenu.reset()
                    }
                }
            }
        }
    }
}