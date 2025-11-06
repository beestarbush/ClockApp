import QtQuick
import QtQuick.Layouts

import Components
import Bee as Backend

Item {
    id: utilities

    GridLayout {
            id: gridLayout
            columns: 2
            width: utilities.width

            Button {
                text: "Shutdown"
                onClicked: Backend.Drivers.system.shutdown()
            }

            Button {
                text: "Reboot"
                onClicked: Backend.Drivers.system.reboot()
            }
    }
}