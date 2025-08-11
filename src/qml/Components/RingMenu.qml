import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property alias model: repeater.model
    property int itemCount: repeater.model.count
    property real itemAngle: 360 / itemCount
    property real rotationAngle: 0
    property int selectedIndex: 0

    onRotationAngleChanged: {
        console.log("ra onchanged: " + rotationAngle)
    }


    signal itemSelected(int index)

    function reset() {
        rotationAngle = 0
        selectedIndex = 0
        itemSelected(selectedIndex)
    }

    Circle {
        id: ring
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        color: "transparent"
        border.color: Qt.rgba(0, 0, 0, 0.6)
        border.width: 100
        border.pixelAligned: true
        
        Repeater {
            id: repeater
            model: menuModel
            delegate: Item {
                width: 60
                height: 20

                property int index: model.index
                property string label: model.label

                property real angle: index * itemAngle - 90
                property real radius: ring.width / 2 - ring.border.width / 2
                property real absoluteAngle: angle + rotationAngle

                x: ring.width / 2 + Math.cos(absoluteAngle * Math.PI / 180) * radius - width / 2
                y: ring.height / 2 + Math.sin(absoluteAngle * Math.PI / 180) * radius - height / 2

                /*transform: Rotation {
                    origin.x: width / 2
                    origin.y: height / 2
                    angle: angle
                }*/

                Text {
                    anchors.centerIn: parent
                    text: label
                    font.pixelSize: 12
                    font.bold: index === selectedIndex
                    color: index === selectedIndex ? "yellow" : "white"

                    transform: Rotation {
                        origin.x: width / 2
                        origin.y: height / 2
                        //angle: -rotationAngle
                        //angle: rotationAngle +90
                        angle: absoluteAngle + 90
                    }

                    Behavior on color {
                        ColorAnimation { duration: 300 }
                    }
                }
            }
        }

        transform: Rotation {
            origin.x: ring.width / 2
            origin.y: ring.height / 2
            angle: rotationAngle
        }
    }

    
MouseArea {
        anchors.fill: parent
        property real lastAngle: 0
        property bool dragging: false

        onPressed: function(event) {
            const dx = event.x - width / 2;
            const dy = event.y - height / 2;
            const distance = Math.sqrt(dx * dx + dy * dy);

            const outerRadius = ring.width / 2;
            const innerRadius = outerRadius - ring.border.width;

            if (distance >= innerRadius && distance <= outerRadius) {
                lastAngle = Math.atan2(dy, dx);
                dragging = true;
            } else {
                dragging = false;
            }
        }

        onPositionChanged: function(event) {
            if (!dragging) return;

            const dx = event.x - width / 2;
            const dy = event.y - height / 2;
            const currentAngle = Math.atan2(dy, dx);
            const delta = (currentAngle - lastAngle) * 180 / Math.PI;

            rotationAngle += delta;
            lastAngle = currentAngle;
        }

onReleased: {
    if (!dragging) return;

    // Normalize rotationAngle to [0, 360)
    let normalizedAngle = ((rotationAngle % 360) + 360) % 360;
    console.log("Normalized angle: " + normalizedAngle)
    console.log("rotationAngle: " + rotationAngle)
    console.log("itemangle: " + itemAngle)

    let rawIndex = Math.abs(rotationAngle / (itemAngle / 2))
    console.log("rawindex: " + rawIndex)
    rawIndex += 0.5
    rawIndex = Math.floor(rawIndex)
    console.log("rawindex: " + rawIndex)

    selectedIndex = rawIndex % itemCount
    rotationAngle = -1 * (selectedIndex * (itemAngle / 2))
    console.log("rotationAngle: " + rotationAngle)
    itemSelected(selectedIndex)

    
/*
    // Compute raw index
    let rawIndex = normalizedAngle / itemAngle;
    console.log("rawIndex: " + rawIndex)

    // Determine direction of rotation
    let snappedIndex;
    if ((rotationAngle - lastAngle * 180 / Math.PI) > 0) {
        // Rotated clockwise → snap forward
        snappedIndex = Math.ceil(rawIndex);
    } else {
        // Rotated counter-clockwise → snap backward
        snappedIndex = Math.floor(rawIndex);
    }
    console.log("snappedIndex: " + snappedIndex)

    snappedIndex = snappedIndex % itemCount;

    // Snap rotation so that this item is at the top
    rotationAngle = snappedIndex * itemAngle;
    console.log("rotationAngle: " + rotationAngle)

    // Update selected index
    selectedIndex = (itemCount - snappedIndex) % itemCount;

    itemSelected(selectedIndex);*/
    dragging = false;
}

    }
}
