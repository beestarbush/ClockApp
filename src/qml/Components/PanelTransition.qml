pragma Singleton
import QtQuick

QtObject {
	id: panelTransition

	property var none: ({
		enable: function() {},
		disable: function() {},
	})

	property var fade: ({
		enable: function(panel) {
			panel.opacity = Qt.binding(function() { return panel.progress })
		},
		disable: function(panel) { panel.opacity = 1.0 },
	})

	property var scale: ({
		enable: function(panel) {
			fade.enable(panel)
			panel.scale = Qt.binding(function() { return panel.progressScale })
		},
		disable: function(panel) {
			fade.disable(panel)
			panel.scale = 1.0
		},
	})

	property var horizontalScrolling: ({
		enable: function(panel) {
			fade.enable(panel)
			panel.anchors.horizontalCenterOffset = Qt.binding(function() { return (1.0 - panel.progress) * panel.panelContainer.direction * panel.direction * Value.largeMargin })
		},
		disable: function(panel) {
			fade.disable(panel)
			panel.anchors.horizontalCenterOffset = 0
		},
	})
}