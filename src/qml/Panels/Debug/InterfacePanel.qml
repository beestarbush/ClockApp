import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Bee as Bee

Item {
    id: interfacesTab
    property string detailsText: ""

    width: parent.width
    height: parent.height

    TreeView {
        id: tree
        width: parent.width * 0.3
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        model: objectModel

        delegate: TreeViewDelegate {

            onClicked: {
                interfacesTab.detailsText =
                    "Name: " + model.display + "\n" +
                    "Type: " + model.type + "\n" +
                    "Value: " + model.value;
            }
        }
    }

    Rectangle {
        color: "#f0f0f0"
        anchors.left: tree.right

        Text {
            text: interfacesTab.detailsText
            wrapMode: TextArea.Wrap
        }
    }
}