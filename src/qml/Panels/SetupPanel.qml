import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: setupPanel

    backgroundColor: Color.black

    property Item lowerMenuOverlay
    property var setup: BeeBackend.Applications.setup

    signal finished()

    // Listen to dial wheel value changes
    Connections {
        target: lowerMenuOverlay
        
        function onDialWheelValueUpdated(newValue) {
            setup.updateDialWheelValue(newValue)
            
            // Update the active DateTimePickerPanel
            if (setup.currentPanel === BeeBackend.SetupEnums.MarriedDateTime) {
                marriedDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === BeeBackend.SetupEnums.KuikenDateTime) {
                kuikenDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === BeeBackend.SetupEnums.CountdownDateTime) {
                countdownDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === BeeBackend.SetupEnums.DeviceId) {
                // Ensure the value always shows 4 digits and prepend SN-
                deviceIdPanel.valueText = `SN-${newValue.toString().padStart(4, '0')}`;
            }
        }
    }

    // React to backend panel changes
    Connections {
        target: setup
        
        function onCurrentPanelChanged() {
            switch(setup.currentPanel) {
                case BeeBackend.SetupEnums.Welcome:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(welcomePanel)
                    break
                case BeeBackend.SetupEnums.DeviceId:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(deviceIdPanel)
                    break
                case BeeBackend.SetupEnums.ServerConnection:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(serverConnectionEnablePanel)
                    break
                case BeeBackend.SetupEnums.MarriedTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedTimerEnablePanel)
                    break
                case BeeBackend.SetupEnums.MarriedDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedDateTimePickerPanel)
                    break
                case BeeBackend.SetupEnums.MarriedBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedBackgroundPickerPanel)
                    break
                case BeeBackend.SetupEnums.KuikenTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenTimerEnablePanel)
                    break
                case BeeBackend.SetupEnums.KuikenDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenDateTimePickerPanel)
                    break
                case BeeBackend.SetupEnums.KuikenBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenBackgroundPickerPanel)
                    break
                case BeeBackend.SetupEnums.CountdownTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownTimerEnablePanel)
                    break
                case BeeBackend.SetupEnums.CountdownDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownDateTimePickerPanel)
                    break
                case BeeBackend.SetupEnums.CountdownBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownBackgroundPickerPanel)
                    break
                case BeeBackend.SetupEnums.Finish:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(finishPanel)
                    break
            }
        }
    }

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: welcomePanel

        currentIndex: {
            switch(setup.currentPanel) {
                case BeeBackend.SetupEnums.Welcome: return indexOfPanel(welcomePanel)
                case BeeBackend.SetupEnums.DeviceId: return indexOfPanel(deviceIdPanel)
                case BeeBackend.SetupEnums.ServerConnection: return indexOfPanel(serverConnectionEnablePanel)
                case BeeBackend.SetupEnums.MarriedTimerEnable: return indexOfPanel(marriedTimerEnablePanel)
                case BeeBackend.SetupEnums.MarriedDateTime: return indexOfPanel(marriedDateTimePickerPanel)
                case BeeBackend.SetupEnums.MarriedBackground: return indexOfPanel(marriedBackgroundPickerPanel)
                case BeeBackend.SetupEnums.KuikenTimerEnable: return indexOfPanel(kuikenTimerEnablePanel)
                case BeeBackend.SetupEnums.KuikenDateTime: return indexOfPanel(kuikenDateTimePickerPanel)
                case BeeBackend.SetupEnums.KuikenBackground: return indexOfPanel(kuikenBackgroundPickerPanel)
                case BeeBackend.SetupEnums.CountdownTimerEnable: return indexOfPanel(countdownTimerEnablePanel)
                case BeeBackend.SetupEnums.CountdownDateTime: return indexOfPanel(countdownDateTimePickerPanel)
                case BeeBackend.SetupEnums.CountdownBackground: return indexOfPanel(countdownBackgroundPickerPanel)
                case BeeBackend.SetupEnums.Finish: return indexOfPanel(finishPanel)
                default: return indexOfPanel(welcomePanel)
            }
        }

        TitlePanel {
            id: welcomePanel

            anchors.fill: parent
            titleText: "Welcome"
            descriptionText: "It's about time you showed up! Let's get things ticking, just follow the steps on screen."
            buttonText: "Next"

            onButtonClicked: setup.next()
        }

        IntegerSelectPanel {
            id: deviceIdPanel

            anchors.fill: parent
            titleText: "Register clock"
            descriptionText: "Tap the value to edit it, then use the dial below to adjust the value."
            valueText: BeeBackend.Services.remoteApi.deviceId
            valueTextSelected: setup.dialWheel.visible

            onValueSelected: setup.showDialWheel(0, 10, 1, 0)

            onButtonClicked: {
                BeeBackend.Services.remoteApi.deviceId = deviceIdPanel.valueText
                setup.next()
            }
        }

        ToggleButtonPanel {
            id: serverConnectionEnablePanel

            anchors.fill: parent
            titleText: "Server connection"
            descriptionText: "Use the toggle to turn a connection with server on or off. This connection can be used to synchronize status of the clock, and add/remove media from the clock."
            toggleTarget: BeeBackend.Services.remoteApi
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        ToggleButtonPanel {
            id: marriedTimerEnablePanel

            anchors.fill: parent
            titleText: "Married timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.marriedTimer
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: marriedDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure married timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.marriedTimer.timestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.marriedTimer.timestamp = selectedTimestamp
                BeeBackend.Applications.marriedTimer.initialized = true
                setup.next()
            }
        }

        TitlePanel {
            id: marriedBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure married timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: setup.next()
        }

        ToggleButtonPanel {
            id: kuikenTimerEnablePanel

            anchors.fill: parent
            titleText: "Kuiken timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.kuikenTimer
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: kuikenDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure kuiken timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.kuikenTimer.timestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.kuikenTimer.timestamp = selectedTimestamp
                BeeBackend.Applications.kuikenTimer.initialized = true
                setup.next()
            }
        }

        TitlePanel {
            id: kuikenBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure kuiken timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: setup.next()
        }

        ToggleButtonPanel {
            id: countdownTimerEnablePanel

            anchors.fill: parent
            titleText: "Countdown timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.countdownTimer
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: countdownDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure countdown timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.countdownTimer.targetTimestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.countdownTimer.targetTimestamp = selectedTimestamp
                BeeBackend.Applications.countdownTimer.initialized = true
                setup.next()
            }
        }

        TitlePanel {
            id: countdownBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure countdown timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: setup.next()
        }

        TitlePanel {
            id: finishPanel

            anchors.fill: parent
            titleText: "All set!"
            descriptionText: "Everything's ticking like clockwork. You're good to go!"
            buttonText: "Let's go!"

            onButtonClicked: {
                setup.finish()
                setupPanel.finished()
            }
        }
    }
}