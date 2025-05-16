import QtQuick
import QtQuick.Layouts

import Components

RoundPanel {
	id: debugPanel

	backgroundColor: Color.lightGray

	signal closeButtonClicked()

	ColumnLayout {
		id: column

		anchors.fill: parent
		anchors.centerIn: parent
		spacing: Value.defaultMargin
		
		Text {
			text: "Button"
			font.bold: true
			//anchors.horizontalCenter: parent.horizontalCenter
			Layout.alignment: Qt.AlignHCenter
		}
		Button {
			text: "Click me"
			onClicked: {
				Backend.greetings()
			}
			Layout.alignment: Qt.AlignHCenter
		}
		
		Text {
			text: "Toggle button"
			font.bold: true
			//anchors.horizontalCenter: parent.horizontalCenter
			Layout.alignment: Qt.AlignHCenter
		}

		ToggleButton {
			Layout.alignment: Qt.AlignHCenter
		}

		Button {
			text: "Close"
			Layout.alignment: Qt.AlignHCenter
			onClicked: debugPanel.closeButtonClicked()
		}
	}
}
