import QtQuick

import Components

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.lightGray

    Pendulum {
        id: pendulum

        bobColor: Backend.pendulumBobColor
        rodColor: Backend.pendulumRodColor
        anchors.fill: parent
    }
}