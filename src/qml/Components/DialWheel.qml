// DialWheel.qml - A simple rotating wheel control
import QtQuick
import QtQuick.Shapes

Item {
    id: dialWheel

    // Configurable properties
    property real minimumValue: 0
    property real maximumValue: 100
    property real stepSize: 1
    property real value: minimumValue
    
    // Visual properties
    property color wheelColor: Color.transparent
    property color markerColor: Color.green1
    property real thickness: 100
    property bool snapToStep: true
    
    // Internal properties
    property real _normalizedValue: (value - minimumValue) / (maximumValue - minimumValue)
    property real _angle: _normalizedValue * 360
    
    width: 200
    height: 200
    
    // Validate value bounds
    onValueChanged: {
        if (value < minimumValue) {
            value = minimumValue
        } else if (value > maximumValue) {
            value = maximumValue
        }
    }
    
    // The rotating wheel
    Item {
        id: wheel
        anchors.fill: parent
        rotation: _angle
        
        Behavior on rotation {
            RotationAnimation {
                duration: 200
                direction: RotationAnimation.Shortest
                easing.type: Easing.OutQuad
            }
        }
        
        // Solid circle
        Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            radius: width / 2
            color: dialWheel.wheelColor
        }
        
        // Position marker at top (12 o'clock position)
        Rectangle {
            width: dialWheel.thickness
            height: dialWheel.thickness
            radius: width / 2
            color: dialWheel.markerColor
            border.color: Qt.lighter(dialWheel.markerColor, 1.3)
            border.width: 3
            
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
        }
    }
    
    // Mouse interaction for rotating the wheel
    MouseArea {
        anchors.fill: parent
        
        property real lastAngle: 0
        property bool isDragging: false
        
        function updateValue(mouse) {
            var centerX = dialWheel.width / 2
            var centerY = dialWheel.height / 2
            var deltaX = mouse.x - centerX
            var deltaY = mouse.y - centerY
            var angle = Math.atan2(deltaY, deltaX) * 180 / Math.PI
            
            // Normalize angle to 0-360 (0 degrees is at 3 o'clock, we want 12 o'clock)
            angle = (angle + 90 + 360) % 360
            
            // Convert angle to value
            var rawValue = minimumValue + (angle / 360) * (maximumValue - minimumValue)
            
            // Apply step size if snap is enabled
            if (snapToStep && stepSize > 0) {
                rawValue = Math.round(rawValue / stepSize) * stepSize
            }
            
            // Clamp to bounds
            rawValue = Math.max(minimumValue, Math.min(maximumValue, rawValue))
            
            dialWheel.value = rawValue
            console.log("Value: " + rawValue)
        }
        
        onPressed: function(mouse) {
            isDragging = true
            updateValue(mouse)
        }
        
        onPositionChanged: function(mouse) {
            if (isDragging) {
                updateValue(mouse)
            }
        }
        
        onReleased: {
            isDragging = false
        }
        
        onWheel: function(wheel) {
            var delta = wheel.angleDelta.y > 0 ? stepSize : -stepSize
            var newValue = dialWheel.value + delta
            newValue = Math.max(minimumValue, Math.min(maximumValue, newValue))
            dialWheel.value = newValue
        }
    }
}
