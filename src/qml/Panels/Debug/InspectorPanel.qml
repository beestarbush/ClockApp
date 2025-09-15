import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Bee as Bee
import Components

Item {
    id: inspectorPanel
    property string detailsText: ""

    width: parent.width
    height: parent.height

    PanelContainer {
        anchors.fill: parent
        currentIndex: indexOfPanel(showModelButton.currentModel ? modelPanel : propertiesPanel)

        Panel {
            id: propertiesPanel

            RoundButton {
                id: showModelButton

                property QtObject currentModel: null

                anchors.top: interfaceSelectorComboBox.top
                anchors.left: parent.left

                text: "Model"

                visible: Utils.isQmlSupportedModel(propertiesListView.currentInterface)

                onClicked: {
                    currentModel = propertiesListView.currentInterface
                }
            }

            ComboBox {
                id: interfaceSelectorComboBox

                property int currentInterfaceIndex: -1
                property var availableInterfaces: Utils.getAvailableInterfaces()

                property bool valid: currentInterfaceIndex >= 0 && currentInterfaceIndex < availableInterfaces.length
                property string currentInterfaceName: valid ? availableInterfaces[currentInterfaceIndex].name : ""
                property QtObject currentInterface: valid ? availableInterfaces[currentInterfaceIndex].interface : null

                anchors.horizontalCenter: parent.horizontalCenter
                width: Value.defaultWidthRatio(parent)

                model: availableInterfaces
                textRole: "name"
                valueRole: "interface"

                displayText: currentInterface ? ("" + currentInterface) : "Select Interface"

                onCurrentIndexChanged: {
                    currentInterfaceIndex = currentIndex
                    propertiesListView.overrideInterface = null
                }

                delegate: ItemDelegate {
                    width: interfaceSelectorComboBox.width
                    contentItem: Text {
                        text: modelData.name
                        color: "black"
                        font: interfaceSelectorComboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: interfaceSelectorComboBox.highlightedIndex === index
                }
            }

            Button {
                anchors.top: interfaceSelectorComboBox.top
                anchors.right: parent.right

                text: "Back"

                visible: !!propertiesListView.overrideInterface

                onClicked: propertiesListView.overrideInterface = null
            }

            ListView {
                id: propertiesListView

                property QtObject currentInterface: overrideInterface ? overrideInterface : interfaceSelectorComboBox.currentInterface
                property QtObject overrideInterface

                anchors.top: interfaceSelectorComboBox.bottom
                anchors.left: parent.left
                anchors.right: propertiesScrollBar.left
                anchors.bottom: parent.bottom
                anchors.margins: Value.defaultMargin

                model: Utils.properties(currentInterface)

                delegate: Item {
                    anchors.left: parent?.left
                    anchors.right: parent?.right
                    height: nameLabel.height + Value.smallMargin * 2

                    property var propInfo: modelData
                    property bool isPointer: Utils.isPointerProperty(propInfo)
                    property QtObject subInterface: isPointer ? Utils.getPointerObject(propertiesListView.currentInterface, propInfo.name) : null

                    MouseArea {
                        id: mouseArea

                        enabled: isPointer && subInterface
                        anchors.left: parent.horizontalCenter
                        anchors.right: valueLabel.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        onClicked: {
                            propertiesListView.overrideInterface = subInterface
                        }

                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            visible: mouseArea.enabled
                            color: mouseArea.pressed ? Color.lightGray : Color.lightGray
                            radius: 4
                            border.width: 1
                            border.color: Color.gray
                        }
                    }

                    Text {
                        id: nameLabel

                        anchors.right: parent.horizontalCenter
                        anchors.rightMargin: Value.defaultMargin / 2
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                        text: propInfo.name + " (" + propInfo.typeName + ")"
                        color: propInfo.isWritable ? "black" : Color.gray
                    }

                    Text {
                        id: valueLabel
                        anchors.left: parent.horizontalCenter
                        anchors.leftMargin: Value.defaultMargin / 2
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        fontSizeMode: Text.Fit

                        Component.onCompleted: {
                            var formatValue = function(value, propInfo) {
                                if (value === null) {
                                    return "null"
                                } else if (value === undefined) {
                                    return "undefined"
                                } else if (value === "") {
                                    return "\"\""
                                } else if (propInfo.isEnumType) {
                                    return Utils.enumValueName(propertiesListView.currentInterface, propInfo.name, propertiesListView.currentInterface[propInfo.name])
                                } else {
                                    return "" + value
                                }
                            }

                            if (propInfo.hasNotifySignal) {
                                text = Qt.binding(function() {
                                    return formatValue(propertiesListView.currentInterface[propInfo.name], propInfo)
                                })
                            } else {
                                text = formatValue(propertiesListView.currentInterface[propInfo.name], propInfo)
                            }
                        }
                    }
                }
            }

            ScrollBar {
                id: propertiesScrollBar

                orientation: Qt.Vertical
                policy: ScrollBar.AsNeeded
                anchors.right: parent.right
                anchors.rightMargin: Value.defaultMargin
                anchors.top: propertiesListView.top
                anchors.bottom: propertiesListView.bottom

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 100
                    radius: width / 2
                    color: propertiesScrollBar.pressed ? Color.darkGray : Color.lightGray
                }
            }

            // Bind ScrollBar to ListView
            Binding {
                target: propertiesScrollBar
                property: "active"
                value: propertiesListView.moving || propertiesScrollBar.pressed
            }
            Binding {
                target: propertiesScrollBar
                property: "position"
                value: propertiesListView.visibleArea.yPosition
            }
            Binding {
                target: propertiesScrollBar
                property: "size"
                value: propertiesListView.visibleArea.heightRatio
            }
        }

        Panel {
            id: modelPanel

            Button {
                id: hideModelButton

                anchors.top: parent.top
                anchors.right: parent.right

                text: "Back"

                onClicked: showModelButton.currentModel = null
            }

            ListView {
                id: modelListView

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: hideModelButton.left
                anchors.bottom: parent.bottom
                anchors.rightMargin: Value.defaultMargin

                model: showModelButton.currentModel

                header: Item {
                    anchors.left: parent?.left
                    anchors.right: parent?.right
                    height: Value.smallButtonHeight

                    RowLayout {
                        anchors.fill: parent
                        spacing: Value.smallMargin

                        Repeater {
                            model: modelListView.roles
                            delegate: Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                implicitWidth: 1
                                implicitHeight: 1

                                Text {
                                    anchors.fill: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    text: modelData
                                    font.bold: true
                                    fontSizeMode: Text.Fit
                                }
                            }
                        }
                    }
                }

                property var roles: Utils.roleNames(model)

                delegate: Item {
                    id: delegate

                    width: ListView.view.width
                    height: Value.smallButtonHeight

                    property QtObject delegateModel: model

                    RowLayout {
                        anchors.fill: parent
                        spacing: Value.smallMargin

                        Repeater {
                            model: modelListView.roles
                            delegate: Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                implicitWidth: 1
                                implicitHeight: 1

                                Text {
                                    anchors.fill: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    text: {
                                        var value = Utils.modelData(modelListView.model, delegate.DelegateModel.itemsIndex, modelData)
                                        var typeInfo = Utils.propertyInformation(delegate.delegateModel, modelData)
                                        return value + "\n" + (typeInfo.variantTypeName || typeInfo.typeName || "")
                                    }
                                    fontSizeMode: Text.Fit
                                    wrapMode: Text.Wrap
                                }
                            }
                        }
                    }
                }
            }

            ScrollBar {
                id: modelScrollBar

                orientation: Qt.Vertical
                policy: ScrollBar.AsNeeded
                anchors.right: parent.right
                anchors.rightMargin: Value.defaultMargin
                anchors.verticalCenter: parent.verticalCenter

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 100
                    radius: width / 2
                    color: modelScrollBar.pressed ? Color.darkGray : Color.lightGray
                }
            }

            // Bind ScrollBar to ListView
            Binding {
                target: modelScrollBar
                property: "active"
                value: modelListView.moving || modelScrollBar.pressed
            }
            Binding {
                target: modelScrollBar
                property: "position"
                value: modelListView.visibleArea.yPosition
            }
            Binding {
                target: modelScrollBar
                property: "size"
                value: modelListView.visibleArea.heightRatio
            }
        }
    }
}
