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
    property real thickness: width / 4
    property bool snapToStep: true
    
    // Internal properties
    property real _normalizedValue: (value - minimumValue) / (maximumValue - minimumValue)
    property real _angle: _normalizedValue * 360
    property real _dragAngle: _angle  // Separate angle for dragging (moves freely)
    
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
        rotation: mouseArea.isDragging ? dialWheel._dragAngle : dialWheel._angle
        
        Behavior on rotation {
            enabled: !mouseArea.isDragging
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
        id: mouseArea
        anchors.fill: parent
        
        property real lastAngle: 0
        property bool isDragging: false
        
        function updateValue(mouse, isRelease) {
            var centerX = dialWheel.width / 2
            var centerY = dialWheel.height / 2
            var deltaX = mouse.x - centerX
            var deltaY = mouse.y - centerY
            var angle = Math.atan2(deltaY, deltaX) * 180 / Math.PI
            
            // Normalize angle to 0-360 (0 degrees is at 3 o'clock, we want 12 o'clock)
            angle = (angle + 90 + 360) % 360
            
            // Convert angle to value
            var rawValue = minimumValue + (angle / 360) * (maximumValue - minimumValue)
            
            // Clamp to bounds
            rawValue = Math.max(minimumValue, Math.min(maximumValue, rawValue))
            
            // While dragging, update the drag angle for smooth rotation
            if (!isRelease) {
                dialWheel._dragAngle = angle
                // Update value continuously during drag; snapping is applied only on release
                dialWheel.value = rawValue
            } else {
                // On release, apply snapping if enabled
                if (snapToStep && stepSize > 0) {
                    rawValue = Math.round(rawValue / stepSize) * stepSize
                    rawValue = Math.max(minimumValue, Math.min(maximumValue, rawValue))
                }
                dialWheel.value = rawValue
            }
        }
        
        onPressed: function(mouse) {
            isDragging = true
            updateValue(mouse, false)
        }
        
        onPositionChanged: function(mouse) {
            if (isDragging) {
                updateValue(mouse, false)
            }
        }
        
        onReleased: function(mouse) {
            updateValue(mouse, true)  // Snap on release
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
