// ColorWheel.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

import Components

Item {
    id: colorWheel
    width: 300
    height: 300

    signal colorSelected(color selectedColor)
    property real rotationAngle: 0
    property real saturation: 1.0
    property real lightness: 0.5
    property bool increaseMode: true
    property color startColor: Color.white
    property color currentColor: Qt.hsla(((360 - rotationAngle + 90 + 180) % 360) / 360, saturation, lightness, 1)

    onStartColorChanged: {
        rotationAngle = ((360 - Math.round(startColor.hslHue * 360) + 90 + 180) % 360)
        // Only update saturation and lightness if they're not extreme values
        // This ensures the wheel remains visible even when starting from white/black
        if (startColor.hslSaturation > 0.1) {
            saturation = startColor.hslSaturation
        }
        if (startColor.hslLightness > 0.1 && startColor.hslLightness < 0.9) {
            lightness = startColor.hslLightness
        }
    }

    Canvas {
        id: canvas
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        rotation: rotationAngle
        renderStrategy: Canvas.Threaded
        renderTarget: Canvas.FramebufferObject

        Component.onCompleted: {
            requestPaint()
        }

        onRotationChanged: {
            requestPaint()
        }

        onPaint: {
            var ctx = getContext("2d")
            var centerX = width / 2
            var centerY = height / 2
            var radius = Math.min(centerX, centerY)

            ctx.clearRect(0, 0, width, height)

            for (var angle = 0; angle < 360; angle += 1) {
                var startAngle = (angle - 1) * Math.PI / 180
                var endAngle = angle * Math.PI / 180
                ctx.beginPath()
                ctx.moveTo(centerX, centerY)
                ctx.arc(centerX, centerY, radius, startAngle, endAngle)
                ctx.closePath()
                ctx.fillStyle = Qt.hsla(angle / 360, colorWheel.saturation, colorWheel.lightness, 1)
                ctx.fill()
            }
        }
    }

    Rectangle {
        id: wheel
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: width / 2
        color: "transparent"
        z: 1

        MouseArea {
            anchors.fill: parent
            property real lastAngle: 0

            onPressed: (mouse) => {
                lastAngle = angleAt(mouse.x, mouse.y)
            }

            onPositionChanged: (mouse) => {
                var newAngle = angleAt(mouse.x, mouse.y)
                var delta = newAngle - lastAngle
                rotationAngle += delta
                rotationAngle = (rotationAngle + 360) % 360
                lastAngle = newAngle

                colorWheel.colorSelected(currentColor)
            }

            function angleAt(x, y) {
                var dx = x - wheel.width / 2
                var dy = y - wheel.height / 2
                var angle = Math.atan2(dy, dx) * 180 / Math.PI
                return angle < 0 ? angle + 360 : angle
            }
        }
    }

    Rectangle {
        id: previewCircle
        width: parent.width / 1.5
        height: width
        radius: width / 2
        color: currentColor
        anchors.centerIn: parent
        z: 3

        MouseArea {
            id: previewMouse
            anchors.fill: parent
            onPressed: {
                increaseMode = !increaseMode
                adjustTimer.start()
            }
            onReleased: {
                adjustTimer.stop()
            }
        }

        Timer {
            id: adjustTimer
            interval: 100
            repeat: true
            running: false
            onTriggered: {
                if (increaseMode) {
                    saturation = Math.min(1.0, saturation + 0.01)
                    lightness = Math.min(1.0, lightness + 0.01)
                } else {
                    saturation = Math.max(0.0, saturation - 0.01)
                    lightness = Math.max(0.0, lightness - 0.01)
                }
                colorWheel.colorSelected(currentColor)
            }
        }
    }

    Rectangle {
        id: topMarker
        width: parent.width / 15
        height: width
        radius: width / 2
        color: "#000000"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        z: 4
    }
}