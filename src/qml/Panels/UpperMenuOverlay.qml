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

        RingMenu {
            id: mainRingMenu
            anchors.centerIn: parent
            anchors.fill: parent          
            model: mainMenuModel

            onItemSelected: (index) => {
                if (index == 1) {
                    settingsRingMenu.visible = true
                }
                else if (index == 3) {
                    // Backgrounds option
                    lowerMenuOverlay.showAnimationSelection()
                }
                else if (index == 5)
                {
                    lowerMenuOverlay.showVersion(Backend.version.tag)
                }
                else {
                    settingsRingMenu.visible = false
                    settingsRingMenu.reset()
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

            Circle {
                anchors.centerIn: parent
                width: settingsRingMenu.visible ? settingsRingMenu.width - 200 : mainRingMenu.width - 200
                height: settingsRingMenu.visible ? settingsRingMenu.height - 200 : mainRingMenu.height - 200
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