import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.black

    Pendulum {
        id: pendulum

        bobColor: BeeBackend.Applications.clock.configuration.pendulumBobColor
        rodColor: BeeBackend.Applications.clock.configuration.pendulumRodColor
        anchors.fill: parent
        active: BeeBackend.Applications.setup.setupComplete || (!BeeBackend.Applications.setup.setupComplete && BeeBackend.Applications.setup.currentPanel === BeeBackend.SetupEnums.Finish)
    }
}