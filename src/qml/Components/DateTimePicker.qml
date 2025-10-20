import QtQuick

import Components

Item {
    id: dateTimePicker
    
    // Public properties
    property date selectedDateTime: new Date()
    property int selectedComponent: 0 // 0=Day, 1=Month, 2=Year, 3=Hour, 4=Minute, 5=Second

    property color selectedTextColor: Color.green1
    property color defaultTextColor: Color.black
    
    // Signals
    signal dateTimeChanged(date newDateTime)
    signal componentSelected(int component)
    
    // Set implicit size based on content
    implicitWidth: dateTimeDisplay.width
    implicitHeight: dateTimeDisplay.height
    
    // DateTime display
    Column {
        id: dateTimeDisplay
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
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
                color: selectedComponent === 0 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 0
                        componentSelected(0)
                    }
                }
            }
            
            Text {
                text: "/"
                font.pixelSize: 48
                color: defaultTextColor
            }
            
            // Month
            Text {
                id: monthText
                text: Qt.formatDate(selectedDateTime, "MM")
                font.pixelSize: 48
                font.bold: selectedComponent === 1
                color: selectedComponent === 1 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 1
                        componentSelected(1)
                    }
                }
            }
            
            Text {
                text: "/"
                font.pixelSize: 48
                color: defaultTextColor
            }
            
            // Year
            Text {
                id: yearText
                text: Qt.formatDate(selectedDateTime, "yyyy")
                font.pixelSize: 48
                font.bold: selectedComponent === 2
                color: selectedComponent === 2 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 2
                        componentSelected(2)
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
                color: selectedComponent === 3 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 3
                        componentSelected(3)
                    }
                }
            }
            
            Text {
                text: ":"
                font.pixelSize: 48
                color: defaultTextColor
            }
            
            // Minutes
            Text {
                id: minutesText
                text: Qt.formatTime(selectedDateTime, "mm")
                font.pixelSize: 48
                font.bold: selectedComponent === 4
                color: selectedComponent === 4 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 4
                        componentSelected(4)
                    }
                }
            }
            
            Text {
                text: ":"
                font.pixelSize: 48
                color: defaultTextColor
            }
            
            // Seconds
            Text {
                id: secondsText
                text: Qt.formatTime(selectedDateTime, "ss")
                font.pixelSize: 48
                font.bold: selectedComponent === 5
                color: selectedComponent === 5 ? selectedTextColor : defaultTextColor
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedComponent = 5
                        componentSelected(5)
                    }
                }
            }
        }
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
