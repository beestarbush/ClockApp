import QtQuick
import QtQuick.Controls

import Components

PanelContainer {
    id: dialogOverlay

    currentIndex: indexOfPanel(mainDialog)

    property Item lowerMenuOverlay

    signal close()

    MenuDialog {
        id: mainDialog

        backgroundOpacity: 0
        signal closeDialog()

        ListModel {
            id: mainMenuModel

            ListElement { label: "Menu" }
            ListElement { label: "Settings" }
            ListElement { label: "Timers" }
            ListElement { label: "Backgrounds" }
            ListElement { label: "Colors" }
            ListElement { label: "Version" }
        }

        ListModel {
            id: settingsMenuModel

            ListElement { label: "Display brightness" }
            ListElement { label: "Background opacity" }
            ListElement { label: "Kuiken timer" }
            ListElement { label: "Marry timer" }
        }

        ListModel {
            id: colorMenuModel

            ListElement { label: "Hours" }
            ListElement { label: "Minutes" }
            ListElement { label: "Seconds" }
            ListElement { label: "Pendulum" }
        }

        RingMenu {
            id: mainRingMenu
            anchors.centerIn: parent
            anchors.fill: parent          
            model: mainMenuModel

            onItemSelected: (index) => {
                if (index == 1) {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = true
                }
                else if (index == 3) {
                    colorRingMenu.visible = false
                    settingsRingMenu.visible = false
                    lowerMenuOverlay.showAnimationSelection()
                }
                else if (index == 4) {
                    settingsRingMenu.visible = false
                    colorRingMenu.visible = true
                }
                else if (index == 5)
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

                visible: mainRingMenu.selectedIndex == 1
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: settingsMenuModel

                function evaluateLowerMenuOverlay(index) {
                    if (index == 0) {
                        lowerMenuOverlay.showScreenBrightnessConfiguration()
                    } else if (index == 1) {
                        lowerMenuOverlay.showBackgroundOpacityConfiguration()
                    } else if (index == 2) {
                        lowerMenuOverlay.showKuikenTimerConfiguration()
                    } else if (index == 3) {
                        lowerMenuOverlay.showMarriedTimerConfiguration()
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

                visible: mainRingMenu.selectedIndex == 4
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: colorMenuModel

                function evaluateLowerMenuOverlay(index) {
                    lower
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