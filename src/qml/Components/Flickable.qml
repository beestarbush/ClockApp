import QtQuick as QQ

import Components

QQ.Flickable {
	id: flickable

	flickDeceleration: Value.flickDeceleration
	boundsBehavior: Value.boundsBehavior
	interactive: contentHeight > height
	clip: interactive
}