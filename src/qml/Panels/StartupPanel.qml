import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
    id: startupPanel

    backgroundColor: Color.blue // This should not be visible, if it is then some formatting is wrong.

    Text {
        id: title

        anchors.centerIn: parent
        color: Color.lightGray
        text: "Starting clock"
    }
}
