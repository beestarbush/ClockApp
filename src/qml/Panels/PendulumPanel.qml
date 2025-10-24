import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.black

    Pendulum {
        id: pendulum

        bobColor: BeeBackend.Applications.clock.pendulumBobColor
        rodColor: BeeBackend.Applications.clock.pendulumRodColor
        anchors.fill: parent
    }
}