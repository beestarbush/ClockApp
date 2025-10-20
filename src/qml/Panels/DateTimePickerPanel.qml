import QtQuick

import Components
import Bee as BeeBackend

RoundPanel {
	id: dateTimePickerPanel

	anchors.fill: parent

    backgroundColor: Color.lightGray

    // State management
    property date selectedDateTime: new Date()
    property int selectedComponent: 0 // 0=Day, 1=Month, 2=Year, 3=Hour, 4=Minute, 5=Second
    
    signal dateTimeChanged(date newDateTime)

    // DateTime display at the top
    Column {
        id: dateTimeDisplay
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.1
        spacing: 15
        
        // Date row (DD/MM/YYYY)
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8
            
            // Day
            Text {
                id: dayText
                text: Qt.formatDate(selectedDateTime, "dd")
                font.pixelSize: 48
                font.bold: selectedComponent === 0
                color: selectedComponent === 0 ? Color.green1 : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 0
                        /*dialWheel.minimumValue = 1
                        dialWheel.maximumValue = getDaysInMonth(selectedDateTime.getFullYear(), selectedDateTime.getMonth())
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getDate()*/
                    }
                }
            }
            
            Text {
                text: "/"
                font.pixelSize: 48
                color: Color.black
            }
            
            // Month
            Text {
                id: monthText
                text: Qt.formatDate(selectedDateTime, "MM")
                font.pixelSize: 48
                font.bold: selectedComponent === 1
                color: selectedComponent === 1 ? Color.orange : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 1
                        /*dialWheel.minimumValue = 1
                        dialWheel.maximumValue = 12
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getMonth() + 1*/
                    }
                }
            }
            
            Text {
                text: "/"
                font.pixelSize: 48
                color: Color.black
            }
            
            // Year
            Text {
                id: yearText
                text: Qt.formatDate(selectedDateTime, "yyyy")
                font.pixelSize: 48
                font.bold: selectedComponent === 2
                color: selectedComponent === 2 ? Color.orange : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 2
                        /*dialWheel.minimumValue = 1970
                        dialWheel.maximumValue = 2100
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getFullYear()*/
                    }
                }
            }
        }
        
        // Time row (HH:MM:SS)
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8
            
            // Hours
            Text {
                id: hoursText
                text: Qt.formatTime(selectedDateTime, "hh")
                font.pixelSize: 48
                font.bold: selectedComponent === 3
                color: selectedComponent === 3 ? Color.orange : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 3
                        /*dialWheel.minimumValue = 0
                        dialWheel.maximumValue = 23
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getHours()*/
                    }
                }
            }
            
            Text {
                text: ":"
                font.pixelSize: 48
                color: Color.black
            }
            
            // Minutes
            Text {
                id: minutesText
                text: Qt.formatTime(selectedDateTime, "mm")
                font.pixelSize: 48
                font.bold: selectedComponent === 4
                color: selectedComponent === 4 ? Color.orange : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 4
                        /*dialWheel.minimumValue = 0
                        dialWheel.maximumValue = 59
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getMinutes()*/
                    }
                }
            }
            
            Text {
                text: ":"
                font.pixelSize: 48
                color: Color.black
            }
            
            // Seconds
            Text {
                id: secondsText
                text: Qt.formatTime(selectedDateTime, "ss")
                font.pixelSize: 48
                font.bold: selectedComponent === 5
                color: selectedComponent === 5 ? Color.orange : Color.black
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 5
                        /*dialWheel.minimumValue = 0
                        dialWheel.maximumValue = 59
                        dialWheel.stepSize = 1
                        dialWheel.value = selectedDateTime.getSeconds()*/
                    }
                }
            }
        }
        
        // Label showing selected component
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: getComponentLabel()
            font.pixelSize: 24
            color: Color.green1
            font.italic: true
        }
    }
    
    // Dial Wheel in the center/bottom area

    
    // Helper functions
    function getComponentLabel() {
        switch(selectedComponent) {
            case 0: return "Day"
            case 1: return "Month"
            case 2: return "Year"
            case 3: return "Hours"
            case 4: return "Minutes"
            case 5: return "Seconds"
        }
        return ""
    }
    
    function updateDateTime(newValue) {
        var newDate = new Date(selectedDateTime)
        
        switch(selectedComponent) {
            case 0: // Day
                newDate.setDate(newValue)
                break
            case 1: // Month
                newDate.setMonth(newValue - 1)
                break
            case 2: // Year
                newDate.setFullYear(newValue)
                break
            case 3: // Hours
                newDate.setHours(newValue)
                break
            case 4: // Minutes
                newDate.setMinutes(newValue)
                break
            case 5: // Seconds
                newDate.setSeconds(newValue)
                break
        }
        
        selectedDateTime = newDate
        dateTimeChanged(newDate)
    }
    
    function getDaysInMonth(year, month) {
        return new Date(year, month + 1, 0).getDate()
    }
}