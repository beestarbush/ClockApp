import QtQuick

Item {
    id: circularLayout

    // The radius of the circle
    property real radius: Math.min(width, height) / 2 - (childWidth / 2)

    // Optional: angle offset in radians (default: start at top)
    property real startAngle: -Math.PI / 2

    // Internal: estimate child width (assumes all children are same size)
    property real childWidth: children.length > 0 ? children[0].width : 0

    // Layout children in a circle
    Component.onCompleted: repositionChildren()
    onWidthChanged: repositionChildren()
    onHeightChanged: repositionChildren()
    onRadiusChanged: repositionChildren()
    onStartAngleChanged: repositionChildren()
    onChildrenChanged: repositionChildren()

    property real spacing: 20 // spacing in pixels along the arc

    function repositionChildren() {
        var N = children.length
        if (N === 0)
            return

        // Place the first child at the top center
        children[0].x = width / 2 - children[0].width / 2
        children[0].y = height / 2 - radius - children[0].height / 2

        if (N === 1)
            return

        // Lay out the rest along the arc, with spacing
        var arcPos = 0 // arc length from the top center
        for (var i = 1; i < N; ++i) {
            // Angle for this arc position
            arcPos += (children[i - 1].width / 2) + spacing + (children[i].width / 2)
            var angle = startAngle + arcPos / radius

            children[i].x = width / 2 + radius * Math.cos(angle) - children[i].width / 2
            children[i].y = height / 2 + radius * Math.sin(angle) - children[i].height / 2
        }
    }
}