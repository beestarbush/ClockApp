import QtQuick
import QtQuick.Controls

import Components
import Bee as BeeBackend

Item {
    id: mediaCarousel

    property var media: []
    property int currentIndex: 0
    property string selectedMedia: media.length > 0 ? media[currentIndex] : ""
    property int selectedIndex: 0

    signal mediaSelected(string mediaName)

    // Watch for media changes and adjust currentIndex if needed
    onMediaChanged: {
        if (currentIndex >= media.length) {
            currentIndex = Math.max(0, media.length - 1)
        }
    }

    function selectCurrentMedia() {
        if (selectedMedia) {
            selectedIndex = currentIndex
            mediaSelected(selectedMedia)
        }
    }

    // Select a media item by its name (exact match). This sets the current page,
    // animates the flickable to that page and emits the `mediaSelected` signal.
    // If the name is not found, nothing happens.
    function selectMediaByName(name) {
        if (!name) return;

        // Extract filename from possible full paths or URLs
        var base = name;
        var lastSlash = Math.max(name.lastIndexOf('/'), name.lastIndexOf('\\'));
        if (lastSlash !== -1) {
            base = name.substring(lastSlash + 1);
        }

        // Strip query params or fragments if present
        var qIdx = base.indexOf('?');
        if (qIdx !== -1) base = base.substring(0, qIdx);
        var hashIdx = base.indexOf('#');
        if (hashIdx !== -1) base = base.substring(0, hashIdx);

        // Now find the index by filename
        var idx = media.indexOf(base);
        if (idx === -1) {
            // Fallback: try matching the full original string (in case model stores full paths)
            idx = media.indexOf(name);
        }
        if (idx === -1) return;

        currentIndex = idx;

        // Smooth scroll to the selected page
        snapAnimation.to = idx * flickable.width
        snapAnimation.start()

        // Emit selection
        selectCurrentMedia()
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
                        source: BeeBackend.Services.media.getMediaPath(mediaName)
                        opacity: 1.0
                        visible: isSelected // Only the selected image is visible (and thus playing).

                        Behavior on opacity {
                            NumberAnimation { duration: 300 }
                        }

                        Circle {
                            anchors.fill: parent

                            color: Color.black
                            opacity: index == selectedIndex ? 0.5 : 0
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
}
