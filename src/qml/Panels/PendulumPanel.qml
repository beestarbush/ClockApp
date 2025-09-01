import QtQuick

import Components

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.lightGray

    Pendulum {
        id: pendulum

        bobColor: Backend.clock.pendulumBobColor
        rodColor: Backend.clock.pendulumRodColor
        anchors.fill: parent
    }
}