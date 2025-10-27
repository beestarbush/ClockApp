import QtQuick
import QtQuick.Layouts

import Components
import Bee as BeeBackend

Item {
    id: utilities

    GridLayout {
            id: gridLayout
            columns: 2
            width: utilities.width

            Button {
                text: "Shutdown"
                onClicked: BeeBackend.HAL.system.shutdown()
            }

            Button {
                text: "Reboot"
                onClicked: BeeBackend.HAL.system.reboot()
            }
    }
}