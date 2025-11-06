import QtQuick

import Components
import Bee as Backend

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.black

    Pendulum {
        id: pendulum

        bobColor: Backend.Applications.clock.configuration.pendulumBobColor
        rodColor: Backend.Applications.clock.configuration.pendulumRodColor
        anchors.fill: parent
        active: Backend.Applications.setup.setupComplete || (!Backend.Applications.setup.setupComplete && Backend.Applications.setup.currentPanel === Backend.SetupEnums.Finish)
    }
}