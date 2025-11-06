import QtQuick

import Components
import Bee as Backend

RoundPanel {
	id: setupPanel

    backgroundColor: Color.black

    property Item lowerMenuOverlay
    property var setup: Backend.Applications.setup

    signal finished()

    // Listen to dial wheel value changes
    Connections {
        target: lowerMenuOverlay
        
        function onDialWheelValueUpdated(newValue) {
            setup.updateDialWheelValue(newValue)
            
            // Update the active DateTimePickerPanel
            if (setup.currentPanel === Backend.SetupEnums.MarriedDateTime) {
                marriedDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === Backend.SetupEnums.KuikenDateTime) {
                kuikenDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === Backend.SetupEnums.CountdownDateTime) {
                countdownDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (setup.currentPanel === Backend.SetupEnums.DeviceId) {
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
                case Backend.SetupEnums.Welcome:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(welcomePanel)
                    break
                case Backend.SetupEnums.DeviceId:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(deviceIdPanel)
                    break
                case Backend.SetupEnums.ServerConnection:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(serverConnectionEnablePanel)
                    break
                case Backend.SetupEnums.MarriedTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedTimerEnablePanel)
                    break
                case Backend.SetupEnums.MarriedDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedDateTimePickerPanel)
                    break
                case Backend.SetupEnums.MarriedBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(marriedBackgroundPickerPanel)
                    break
                case Backend.SetupEnums.KuikenTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenTimerEnablePanel)
                    break
                case Backend.SetupEnums.KuikenDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenDateTimePickerPanel)
                    break
                case Backend.SetupEnums.KuikenBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(kuikenBackgroundPickerPanel)
                    break
                case Backend.SetupEnums.CountdownTimerEnable:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownTimerEnablePanel)
                    break
                case Backend.SetupEnums.CountdownDateTime:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownDateTimePickerPanel)
                    break
                case Backend.SetupEnums.CountdownBackground:
                    panelContainer.currentIndex = panelContainer.indexOfPanel(countdownBackgroundPickerPanel)
                    break
                case Backend.SetupEnums.Finish:
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
            if (!setup) return indexOfPanel(welcomePanel)
            switch(setup.currentPanel) {
                case Backend.SetupEnums.Welcome: return indexOfPanel(welcomePanel)
                case Backend.SetupEnums.DeviceId: return indexOfPanel(deviceIdPanel)
                case Backend.SetupEnums.ServerConnection: return indexOfPanel(serverConnectionEnablePanel)
                case Backend.SetupEnums.MarriedTimerEnable: return indexOfPanel(marriedTimerEnablePanel)
                case Backend.SetupEnums.MarriedDateTime: return indexOfPanel(marriedDateTimePickerPanel)
                case Backend.SetupEnums.MarriedBackground: return indexOfPanel(marriedBackgroundPickerPanel)
                case Backend.SetupEnums.KuikenTimerEnable: return indexOfPanel(kuikenTimerEnablePanel)
                case Backend.SetupEnums.KuikenDateTime: return indexOfPanel(kuikenDateTimePickerPanel)
                case Backend.SetupEnums.KuikenBackground: return indexOfPanel(kuikenBackgroundPickerPanel)
                case Backend.SetupEnums.CountdownTimerEnable: return indexOfPanel(countdownTimerEnablePanel)
                case Backend.SetupEnums.CountdownDateTime: return indexOfPanel(countdownDateTimePickerPanel)
                case Backend.SetupEnums.CountdownBackground: return indexOfPanel(countdownBackgroundPickerPanel)
                case Backend.SetupEnums.Finish: return indexOfPanel(finishPanel)
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
            valueText: Backend.Services.remoteApi.deviceId
            valueTextSelected: setup && setup.dialWheel ? setup.dialWheel.visible : false

            onValueSelected: setup.showDialWheel(0, 10, 1, 0)

            onButtonClicked: {
                Backend.Services.remoteApi.deviceId = deviceIdPanel.valueText
                setup.next()
            }
        }

        ToggleButtonPanel {
            id: serverConnectionEnablePanel

            anchors.fill: parent
            titleText: "Server connection"
            descriptionText: "Use the toggle to turn a connection with server on or off. This connection can be used to synchronize status of the clock, and add/remove media from the clock."
            toggleTarget: Backend.Services.remoteApi
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        ToggleButtonPanel {
            id: marriedTimerEnablePanel

            anchors.fill: parent
            titleText: "Married timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: Backend.Applications.marriedTimer.configuration
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: marriedDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure married timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(Backend.Applications.marriedTimer.configuration.timestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                Backend.Applications.marriedTimer.configuration.timestamp = selectedTimestamp
                Backend.Applications.marriedTimer.configuration.initialized = true
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
            toggleTarget: Backend.Applications.kuikenTimer.configuration
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: kuikenDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure kuiken timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(Backend.Applications.kuikenTimer.configuration.timestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                Backend.Applications.kuikenTimer.configuration.timestamp = selectedTimestamp
                Backend.Applications.kuikenTimer.configuration.initialized = true
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
            toggleTarget: Backend.Applications.countdownTimer.configuration
            toggleProperty: "enabled"

            onButtonClicked: setup.next()
        }

        DateTimePickerPanel {
            id: countdownDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure countdown timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(Backend.Applications.countdownTimer.configuration.timestamp * 1000)

            onComponentSelected: function(component) {
                var d = dateTime
                setup.showDateTimeComponentPicker(component, d.getFullYear(), d.getMonth() + 1, 
                    d.getDate(), d.getHours(), d.getMinutes(), d.getSeconds())
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                Backend.Applications.countdownTimer.configuration.timestamp = selectedTimestamp
                Backend.Applications.countdownTimer.configuration.initialized = true
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