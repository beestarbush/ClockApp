import QtQuick
import QtQuick.Controls

import Components
import Bee

Item {
    id: animationCarousel

    property var animations: []
    property int currentIndex: 0
    property string selectedAnimation: animations.length > 0 ? animations[currentIndex] : ""

    signal animationSelected(string animationName)

    function setAnimations(animationList) {
        animations = animationList || []
        if (currentIndex >= animations.length) {
            currentIndex = Math.max(0, animations.length - 1)
        }
    }

    function selectCurrentAnimation() {
        if (selectedAnimation) {
            animationSelected(selectedAnimation)
        }
    }

    // No animations message
    Text {
        id: noAnimationsText
        anchors.centerIn: parent
        text: "No animations found\nPlace GIF files in animations directory"
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        horizontalAlignment: Text.AlignHCenter
        visible: animations.length === 0
    }

    // Main flickable for swiping through animations
    Flickable {
        id: flickable
        anchors.fill: parent
        
        contentWidth: animationRow.width
        contentHeight: height
        flickDeceleration: 1500
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        interactive: animations.length > 1
        
        visible: animations.length > 0

        Row {
            id: animationRow
            height: parent.height
            spacing: 0

            Repeater {
                model: animations

                delegate: Item {
                    id: animationItem
                    width: flickable.width
                    height: flickable.height

                    property bool isSelected: index === currentIndex
                    property string animationName: modelData

                    // Full screen animation preview
                    RoundAnimatedImage {
                        id: previewImage
                        anchors.fill: parent
                        source: AnimationManager.getAnimationPath(animationName)
                        opacity: isSelected ? 1.0 : 0.3
                        visible: isSelected // Only the selected image is visible (and thus playing).

                        Behavior on opacity {
                            NumberAnimation { duration: 300 }
                        }
                    }

                    // Animation name overlay
                    Rectangle {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: Value.defaultMargin * 2
                        width: nameText.width + Value.defaultMargin
                        height: nameText.height + Value.smallMargin
                        color: Color.black
                        opacity: 0.8
                        radius: Value.smallMargin
                        visible: isSelected

                        Text {
                            id: nameText
                            anchors.centerIn: parent
                            text: animationName
                            color: Color.lightGray
                            font.pixelSize: Value.defaultTextSize
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    // Click to select
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentIndex = index
                            selectCurrentAnimation()
                        }
                    }
                }
            }
        }

        // Snap to pages when flicking stops
        onMovementEnded: {
            var targetIndex = Math.round(contentX / width)
            targetIndex = Math.max(0, Math.min(targetIndex, animations.length - 1))
            
            if (targetIndex !== currentIndex) {
                currentIndex = targetIndex
            }
            
            // Smooth scroll to the selected page
            snapAnimation.to = targetIndex * width
            snapAnimation.start()
        }

        NumberAnimation {
            id: snapAnimation
            target: flickable
            property: "contentX"
            duration: 300
            easing.type: Easing.OutCubic
        }
    }

    // Page indicators at bottom
    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: Value.smallMargin
        spacing: Value.smallMargin
        visible: animations.length > 1

        Repeater {
            model: animations.length

            delegate: Rectangle {
                width: 15
                height: width
                radius: width / 2
                color: index === currentIndex ? Color.green1 : Color.gray
                opacity: index === currentIndex ? 1.0 : 0.5

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }
            }
        }
    }

    // Navigation arrows for additional clarity
    Rectangle {
        id: leftArrow
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: Value.defaultMargin
        width: Value.defaultMargin * 2
        height: width
        radius: width / 2
        color: Color.black
        opacity: currentIndex > 0 ? 0.7 : 0.3
        visible: animations.length > 1

        Text {
            anchors.centerIn: parent
            text: "‹"
            color: Color.lightGray
            font.pixelSize: Value.largeTextSize
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: currentIndex > 0
            onClicked: {
                currentIndex = Math.max(0, currentIndex - 1)
                snapAnimation.to = currentIndex * flickable.width
                snapAnimation.start()
            }
        }
    }

    Rectangle {
        id: rightArrow
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: Value.defaultMargin
        width: Value.defaultMargin * 2
        height: width
        radius: width / 2
        color: Color.black
        opacity: currentIndex < animations.length - 1 ? 0.7 : 0.3
        visible: animations.length > 1

        Text {
            anchors.centerIn: parent
            text: "›"
            color: Color.lightGray
            font.pixelSize: Value.largeTextSize
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: currentIndex < animations.length - 1
            onClicked: {
                currentIndex = Math.min(animations.length - 1, currentIndex + 1)
                snapAnimation.to = currentIndex * flickable.width
                snapAnimation.start()
            }
        }
    }

    // Update animations when Backend properties change
    Connections {
        target: Backend
        function onAvailableAnimationsChanged() {
            setAnimations(Backend.availableAnimations)
        }
    }

    Component.onCompleted: {
        setAnimations(Backend.availableAnimations)
    }
}