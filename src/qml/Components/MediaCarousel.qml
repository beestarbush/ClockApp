import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

Item {
    id: mediaCarousel

    property var media: []
    property int currentIndex: 0
    property string selectedMedia: media.length > 0 ? media[currentIndex] : ""

    signal mediaSelected(string mediaName)

    function setMedia(mediaList) {
        media = mediaList || []
        if (currentIndex >= media.length) {
            currentIndex = Math.max(0, media.length - 1)
        }
    }

    function selectCurrentMedia() {
        if (selectedMedia) {
            mediaSelected(selectedMedia)
        }
    }

    // No media message
    Text {
        id: noMediaText
        anchors.centerIn: parent
        text: "No media found\nPlace files in media directory"
        color: Color.lightGray
        font.pixelSize: Value.defaultTextSize
        horizontalAlignment: Text.AlignHCenter
        visible: media.length === 0
    }

    // Main flickable for swiping through media
    Flickable {
        id: flickable
        anchors.fill: parent
        
        contentWidth: mediaRow.width
        contentHeight: height
        flickDeceleration: 1500
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        interactive: media.length > 1
        
        visible: media.length > 0

        Row {
            id: mediaRow
            height: parent.height
            spacing: 0

            Repeater {
                model: media

                delegate: Item {
                    id: mediaItem
                    width: flickable.width
                    height: flickable.height

                    property bool isSelected: index === currentIndex
                    property string mediaName: modelData

                    // Full screen media preview
                    BeeBackend.RoundAnimatedImage {
                        id: previewImage
                        anchors.fill: parent
                        source: BeeBackend.Services.mediaManager.getMediaPath(mediaName)
                        opacity: isSelected ? 1.0 : 0.3
                        visible: isSelected // Only the selected image is visible (and thus playing).

                        Behavior on opacity {
                            NumberAnimation { duration: 300 }
                        }
                    }

                    // Media name overlay
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
                            text: mediaName
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
                            selectCurrentMedia()
                        }
                    }
                }
            }
        }

        // Snap to pages when flicking stops
        onMovementEnded: {
            var targetIndex = Math.round(contentX / width)
            targetIndex = Math.max(0, Math.min(targetIndex, media.length - 1))
            
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
        visible: media.length > 1

        Repeater {
            model: media.length

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
        visible: media.length > 1

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
        opacity: currentIndex < media.length - 1 ? 0.7 : 0.3
        visible: media.length > 1

        Text {
            anchors.centerIn: parent
            text: "›"
            color: Color.lightGray
            font.pixelSize: Value.largeTextSize
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: currentIndex < media.length - 1
            onClicked: {
                currentIndex = Math.min(media.length - 1, currentIndex + 1)
                snapAnimation.to = currentIndex * flickable.width
                snapAnimation.start()
            }
        }
    }

    // Update media when Backend properties change
    Connections {
        target: Backend
        function onAvailableMediaChanged() {
            setMedia(Backend.availableMedia)
        }
    }

    Component.onCompleted: {
        setMedia(Backend.availableMedia)
    }
}
