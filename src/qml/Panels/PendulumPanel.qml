import QtQuick

import Components

RoundPanel {
	id: pendulumPanel

	anchors.fill: parent

    backgroundColor: Color.lightGray

    Pendulum {
        id: pendulum

        anchors.fill: parent
    }
}