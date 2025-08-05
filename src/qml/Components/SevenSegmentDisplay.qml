import QtQuick 2.15

Item {
    id: display
    property int digit: 0
    property color color: Color.darkGray
    property color offColor: Color.darkGray
    width: 100
    height: 200

    // Segment visibility map for digits 0-9
    property var segmentMap: [
        [true, true, true, true, true, true, false],  // 0
        [false, true, true, false, false, false, false],  // 1
        [true, true, false, true, true, false, true],  // 2
        [true, true, true, true, false, false, true],  // 3
        [false, true, true, false, false, true, true],  // 4
        [true, false, true, true, false, true, true],  // 5
        [true, false, true, true, true, true, true],  // 6
        [true, true, true, false, false, false, false],  // 7
        [true, true, true, true, true, true, true],  // 8
        [true, true, true, true, false, true, true]   // 9
    ]

    property real segmentRadius: width / 50

    property real horizontalSegmentWidth: width / 1.67
    property real horizontalSegmentHeight: height / 20
    property real horizontalSegmentXStart: width / 5
    property real horiziontalSegmentUpperYStart: 0
    property real horiziontalSegmentMiddleYStart: horiziontalSegmentUpperYStart + verticalSegmentUpperYStart + verticalSegmentHeight
    property real horiziontalSegmentLowerYStart: horiziontalSegmentUpperYStart + verticalSegmentUpperYStart + verticalSegmentHeight + horizontalSegmentHeight + verticalSegmentHeight

    property real verticalSegmentWidth: width / 10
    property real verticalSegmentHeight: height / 3.33
    property real verticalSegmentLeftXStart: width / 10
    property real verticalSegmentRightXStart: (width - verticalSegmentLeftXStart - verticalSegmentWidth)
    property real verticalSegmentUpperYStart: height / 20
    property real verticalSegmentLowerYStart: verticalSegmentUpperYStart + verticalSegmentHeight + horizontalSegmentHeight
    
    // Segment definitions
    Rectangle { id: segA; radius: segmentRadius; width: horizontalSegmentWidth; height: horizontalSegmentHeight; x: horizontalSegmentXStart; y: horiziontalSegmentUpperYStart; color: segmentMap[digit][0] ? display.color : display.offColor; visible: true }
    Rectangle { id: segB; radius: segmentRadius; width: verticalSegmentWidth; height: verticalSegmentHeight; x: verticalSegmentRightXStart; y: verticalSegmentUpperYStart; color: segmentMap[digit][1] ? display.color : display.offColor; visible: true }
    Rectangle { id: segC; radius: segmentRadius; width: verticalSegmentWidth; height: verticalSegmentHeight; x: verticalSegmentRightXStart; y: verticalSegmentLowerYStart; color: segmentMap[digit][2] ? display.color : display.offColor; visible: true }
    Rectangle { id: segD; radius: segmentRadius; width: horizontalSegmentWidth; height: horizontalSegmentHeight; x: horizontalSegmentXStart; y: horiziontalSegmentLowerYStart; color: segmentMap[digit][3] ? display.color : display.offColor; visible: true }
    Rectangle { id: segE; radius: segmentRadius; width: verticalSegmentWidth; height: verticalSegmentHeight; x: verticalSegmentLeftXStart; y: verticalSegmentLowerYStart; color: segmentMap[digit][4] ? display.color : display.offColor; visible: true }
    Rectangle { id: segF; radius: segmentRadius; width: verticalSegmentWidth; height: verticalSegmentHeight; x: verticalSegmentLeftXStart; y: verticalSegmentUpperYStart; color: segmentMap[digit][5] ? display.color : display.offColor; visible: true }
    Rectangle { id: segG; radius: segmentRadius; width: horizontalSegmentWidth; height: horizontalSegmentHeight; x: horizontalSegmentXStart; y: horiziontalSegmentMiddleYStart; color: segmentMap[digit][6] ? display.color : display.offColor; visible: true }
}
