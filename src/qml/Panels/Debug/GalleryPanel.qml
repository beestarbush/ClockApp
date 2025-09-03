import QtQuick
import QtQuick.Layouts

import Components
import Bee

Item {
    id: galleryTab

    ColumnLayout {
        anchors.fill: parent

        Button {}

        RoundButton{}

        Text {
            text: HAL.temperature.processorTemperature
        }
    }
}