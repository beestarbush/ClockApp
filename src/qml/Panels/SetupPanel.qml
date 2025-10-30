import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: setupPanel

    backgroundColor: Color.black

    property Item lowerMenuOverlay

    signal finished()

    // Listen to dial wheel value changes
    Connections {
        target: lowerMenuOverlay
        
        function onDialWheelValueUpdated(newValue) {
            // Update the active DateTimePickerPanel
            if (panelContainer.currentIndex == panelContainer.indexOfPanel(marriedDateTimePickerPanel)) {
                marriedDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (panelContainer.currentIndex == panelContainer.indexOfPanel(kuikenDateTimePickerPanel)) {
                kuikenDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (panelContainer.currentIndex == panelContainer.indexOfPanel(countdownDateTimePickerPanel)) {
                countdownDateTimePickerPanel.updateSelectedComponent(newValue)
            } else if (panelContainer.currentIndex == panelContainer.indexOfPanel(deviceIdPanel)) {
                // Ensure the value always shows 4 digits and prepend SN-
                deviceIdPanel.valueText = `SN-${newValue.toString().padStart(4, '0')}`;
            }
        }
    }

    PanelContainer {
        id: panelContainer
        anchors.fill: parent

        property Panel initialPanel: welcomePanel

        currentIndex: indexOfPanel(welcomePanel)

        TitlePanel {
            id: welcomePanel

            anchors.fill: parent
            titleText: "Welcome"
            descriptionText: "It’s about time you showed up! Let’s get things ticking, just follow the steps on screen."
            buttonText: "Next"

            onButtonClicked: {
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(deviceIdPanel)
            }
        }

        IntegerSelectPanel {
            id: deviceIdPanel

            anchors.fill: parent
            titleText: "Register clock"
            descriptionText: "Tap the value to edit it, then use the dial below to adjust the value."
            valueText: BeeBackend.Services.remoteApi.deviceId
            valueTextSelected: lowerMenuOverlay.visible

            onValueSelected: {
                lowerMenuOverlay.visible = true
                lowerMenuOverlay.showDialWheel(0, 10, 1, 0)
            }

            onButtonClicked: {
                lowerMenuOverlay.visible = false

                BeeBackend.Services.remoteApi.deviceId = deviceIdPanel.valueText

                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(serverConnectionEnablePanel)
            }
        }

        ToggleButtonPanel {
            id: serverConnectionEnablePanel

            anchors.fill: parent
            titleText: "Server connection"
            descriptionText: "Use the toggle to turn a connection with server on or off. This connection can be used to synchronize status of the clock, and add/remove media from the clock."
            toggleTarget: BeeBackend.Services.remoteApi
            toggleProperty: "enabled"

            onButtonClicked: {
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(marriedTimerEnablePanel)
            }
        }

        ToggleButtonPanel {
            id: marriedTimerEnablePanel

            anchors.fill: parent
            titleText: "Married timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.marriedTimer
            toggleProperty: "enabled"

            onButtonClicked: {
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(marriedDateTimePickerPanel)
            }
        }

        DateTimePickerPanel {
            id: marriedDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure married timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.marriedTimer.timestamp * 1000)

            onComponentSelected: function(component) {
                lowerMenuOverlay.visible = true
                var params = getDialWheelParams(marriedDateTimePickerPanel, component)
                lowerMenuOverlay.showDialWheel(params.min, params.max, params.step, params.value)
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.marriedTimer.timestamp = selectedTimestamp
                BeeBackend.Applications.marriedTimer.initialized = true

                BeeBackend.Applications.setup.nextSetupStep()

                // Move to next panel
                lowerMenuOverlay.visible = false
                panelContainer.showPanel(marriedBackgroundPickerPanel)

                lowerMenuOverlay.visible = true
                lowerMenuOverlay.showMediaSelection(1)
            }
        }

        TitlePanel {
            id: marriedBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure married timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: {
                lowerMenuOverlay.visible = false
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(kuikenTimerEnablePanel)
            }
        }

        ToggleButtonPanel {
            id: kuikenTimerEnablePanel

            anchors.fill: parent
            titleText: "Kuiken timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.kuikenTimer
            toggleProperty: "enabled"

            onButtonClicked: {
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(kuikenDateTimePickerPanel)
            }
        }

        DateTimePickerPanel {
            id: kuikenDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure kuiken timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.kuikenTimer.timestamp * 1000)

            onComponentSelected: function(component) {
                lowerMenuOverlay.visible = true
                var params = getDialWheelParams(kuikenDateTimePickerPanel, component)
                lowerMenuOverlay.showDialWheel(params.min, params.max, params.step, params.value)
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.kuikenTimer.timestamp = selectedTimestamp
                BeeBackend.Applications.kuikenTimer.initialized = true

                BeeBackend.Applications.setup.nextSetupStep()

                // Move to next panel
                lowerMenuOverlay.visible = false
                panelContainer.showPanel(kuikenBackgroundPickerPanel)

                lowerMenuOverlay.visible = true
                lowerMenuOverlay.showMediaSelection(2)
            }
        }

        TitlePanel {
            id: kuikenBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure kuiken timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: {
                lowerMenuOverlay.visible = false
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(countdownTimerEnablePanel)
            }
        }

        ToggleButtonPanel {
            id: countdownTimerEnablePanel

            anchors.fill: parent
            titleText: "Countdown timer setting"
            descriptionText: "Use the toggle to turn the timer on or off."
            toggleTarget: BeeBackend.Applications.countdownTimer
            toggleProperty: "enabled"

            onButtonClicked: {
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(countdownDateTimePickerPanel)
            }
        }

        DateTimePickerPanel {
            id: countdownDateTimePickerPanel

            anchors.fill: parent
            titleText: "Configure countdown timer"
            descriptionText: "Tap a part of the date or time to edit it, then use the dial below to adjust the value."
            dateTime: new Date(BeeBackend.Applications.countdownTimer.targetTimestamp * 1000)

            onComponentSelected: function(component) {
                lowerMenuOverlay.visible = true
                var params = getDialWheelParams(countdownDateTimePickerPanel, component)
                lowerMenuOverlay.showDialWheel(params.min, params.max, params.step, params.value)
            }

            onButtonClicked: {
                var selectedTimestamp = Math.floor(dateTime.getTime() / 1000)
                BeeBackend.Applications.countdownTimer.targetTimestamp = selectedTimestamp
                BeeBackend.Applications.countdownTimer.initialized = true

                BeeBackend.Applications.setup.nextSetupStep()

                // Move to next panel
                lowerMenuOverlay.visible = false
                panelContainer.showPanel(countdownBackgroundPickerPanel)

                lowerMenuOverlay.visible = true
                lowerMenuOverlay.showMediaSelection(3)
            }
        }

        TitlePanel {
            id: countdownBackgroundPickerPanel

            anchors.fill: parent
            titleText: "Configure countdown timer background"
            descriptionText: "Select a background below."
            buttonText: "Next"

            onButtonClicked: {
                lowerMenuOverlay.visible = false
                BeeBackend.Applications.setup.nextSetupStep()
                panelContainer.showPanel(finishPanel)
            }
        }

        TitlePanel {
            id: finishPanel

            anchors.fill: parent
            titleText: "All set!"
            descriptionText: "Everything’s ticking like clockwork. You’re good to go!"
            buttonText: "Let's go!"

            onButtonClicked: {
                BeeBackend.Applications.setup.markSetupComplete()
                setupPanel.finished()

                lowerMenuOverlay.closePanels()
                panelContainer.showPanel(welcomePanel)
            }
        }
    }

    function getDialWheelParams(dateTimePickerPanel, component) {
        var params = { min: 0, max: 100, step: 1, value: 0 }
        var currentDate = dateTimePickerPanel.dateTime
        
        switch(component) {
            case 0: // Day
                params.min = 1
                params.max = getDaysInMonth(currentDate.getFullYear(), currentDate.getMonth())
                params.step = 1
                params.value = currentDate.getDate()
                break
            case 1: // Month
                params.min = 1
                params.max = 12
                params.step = 1
                params.value = currentDate.getMonth() + 1
                break
            case 2: // Year
                params.min = 1970
                params.max = 2100
                params.step = 1
                params.value = currentDate.getFullYear()
                break
            case 3: // Hours
                params.min = 0
                params.max = 23
                params.step = 1
                params.value = currentDate.getHours()
                break
            case 4: // Minutes
                params.min = 0
                params.max = 59
                params.step = 1
                params.value = currentDate.getMinutes()
                break
            case 5: // Seconds
                params.min = 0
                params.max = 59
                params.step = 1
                params.value = currentDate.getSeconds()
                break
        }
        
        return params
    }

    function getDaysInMonth(year, month) {
        return new Date(year, month + 1, 0).getDate()
    }
}