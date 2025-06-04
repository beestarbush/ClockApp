import QtQuick
import QtQuick.Shapes

Shape {
    id: progressBar

    property alias thickness: shapePath.strokeWidth
    property int progress: 0
    property int divisor: 60
    property alias color: shapePath.strokeColor
    property alias showLabel: progressValue.visible

    ShapePath {
        id: shapePath
        strokeWidth: thickness
        strokeColor: "orange"
        capStyle: ShapePath.FlatCap
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"

        PathAngleArc {
            centerX: width / 2
            centerY: height / 2
            radiusX: width / 2 - thickness / 2
            radiusY: height / 2 - thickness / 2
            startAngle: -90
            sweepAngle: progress / divisor * 360

            Behavior on sweepAngle {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Text {
        id: progressValue

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        visible: true
        font.pointSize: timeSincePanel.width > 0 ? timeSincePanel.width * 0.04 : Value.defaultTextSize
        text: "%1".arg(progress)
        color: Qt.darker(shapePath.strokeColor, 1.3)
    }
}