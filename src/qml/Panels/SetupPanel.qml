import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: setupPanel

    backgroundColor: Color.black

    signal finished()

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: welcomePanel

        currentIndex: indexOfPanel(welcomePanel)

        TitlePanel {
            id: welcomePanel

            anchors.fill: parent
            titleText: "Welcome"
            buttonText: "Next"

            onButtonClicked: panelContainer.showPanel(marriedDateTimePickerPanel)
        }

        DateTimePickerPanel {
            id: marriedDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure Married timer"
            dateTime: new Date(BeeBackend.Applications.marriedTimer.timestamp * 1000)

            onButtonClicked: panelContainer.showPanel(kuikenDateTimePickerPanel)
        }

        DateTimePickerPanel {
            id: kuikenDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure Kuiken timer"
            dateTime: new Date(BeeBackend.Applications.kuikenTimer.timestamp * 1000)

            onButtonClicked: panelContainer.showPanel(finishPanel)
        }

        TitlePanel {
            id: finishPanel

            anchors.fill: parent
            titleText: "Clock is set up!"
            buttonText: "Finish"

            onButtonClicked: setupPanel.finished()
        }
    }
}