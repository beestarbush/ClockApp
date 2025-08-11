import QtQuick
import QtQuick.Controls

import Bee as Bee

Item {
    id: interfacesTab

    property var componentMap: ({})
    Component.onCompleted: {
        for (let i = 0; i < Bee.QmlInterface.registeredObjectsNames.length; ++i) {
            let name = Bee.QmlInterface.registeredObjectsNames[i]
            if (Bee[name] !== undefined) {
                componentMap[name] = Bee[name]
            }
        }
        combo.model = Object.keys(componentMap)
    }

    Column {
        anchors.fill: parent
        spacing: 10
        padding: 10

        ComboBox {
            id: combo
            model: Object.keys(interfacesTab.componentMap)
            onCurrentTextChanged: {
                propertyList.model = interfacesTab.getProperties(interfacesTab.componentMap[currentText])
            }
        }

        Loader {
            id: loader
            width: 200
            height: 100
        }

        ListView {
            id: propertyList
            width: parent.width
            height: 200
            model: interfacesTab.getProperties(loader.item)
            delegate: Text {
                text: modelData
            }
        }
    }

    function getProperties(item) {
        if (!item)
            return []

        let props = []
        for (let prop in item) {
            if (item.hasOwnProperty(prop) && typeof item[prop] !== "function") {
                props.push(`${prop}: ${item[prop]}`)
            }
        }
        return props
    }
}