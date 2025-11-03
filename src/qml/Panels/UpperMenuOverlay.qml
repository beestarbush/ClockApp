import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

PanelContainer {
    id: overlay

    currentIndex: indexOfPanel(mainDialog)

    signal close()

    MenuDialog {
        id: mainDialog
        backgroundOpacity: 0

        RingMenu {
            id: mainRingMenu
            anchors.centerIn: parent
            anchors.fill: parent
            model: BeeBackend.Applications.menu.main

            onItemSelected: (index) => {
                BeeBackend.Applications.menu.main.get(index).trigger()
            }

            RingMenu {
                id: settingsRingMenu
                visible: mainRingMenu.selectedIndex === BeeBackend.MenuEnums.MainSettings
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: BeeBackend.Applications.menu.settings

                onItemSelected: (index) => {
                    BeeBackend.Applications.menu.settings.get(index).trigger()
                }

                onVisibleChanged: {
                    if (visible && settingsRingMenu.selectedIndex >= 0) {
                        BeeBackend.Applications.menu.settings.get(settingsRingMenu.selectedIndex).trigger()
                    }
                }
            }

            RingMenu {
                id: colorsRingMenu
                visible: mainRingMenu.selectedIndex === BeeBackend.MenuEnums.MainColors
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: BeeBackend.Applications.menu.colors

                onItemSelected: (index) => {
                    BeeBackend.Applications.menu.colors.get(index).trigger()
                }

                onVisibleChanged: {
                    if (visible && colorsRingMenu.selectedIndex >= 0) {
                        BeeBackend.Applications.menu.colors.get(colorsRingMenu.selectedIndex).trigger()
                    }
                }
            }

            RingMenu {
                id: backgroundsRingMenu
                visible: mainRingMenu.selectedIndex === BeeBackend.MenuEnums.MainBackgrounds
                anchors.centerIn: parent
                width: parent.width - 200
                height: parent.height - 200
                model: BeeBackend.Applications.menu.backgrounds

                onItemSelected: (index) => {
                    BeeBackend.Applications.menu.backgrounds.get(index).trigger()
                }

                onVisibleChanged: {
                    if (visible && backgroundsRingMenu.selectedIndex >= 0) {
                        BeeBackend.Applications.menu.backgrounds.get(backgroundsRingMenu.selectedIndex).trigger()
                    }
                }
            }

            Circle {
                anchors.centerIn: parent
                width: (settingsRingMenu.visible || colorsRingMenu.visible || backgroundsRingMenu.visible) 
                    ? settingsRingMenu.width - 200 
                    : mainRingMenu.width - 200
                height: width
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        overlay.close()
                        BeeBackend.Applications.menu.closeDialog()
                        mainRingMenu.reset()
                    }
                }
            }
        }
    }
}