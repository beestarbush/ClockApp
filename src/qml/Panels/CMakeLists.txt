qt_add_library(PanelLib STATIC)

set_target_properties(PanelLib PROPERTIES AUTOMOC ON)
target_link_libraries(PanelLib PRIVATE Qt6::Quick)

qt_add_qml_module(PanelLib
    URI Panels
    VERSION 1.0
    QML_FILES
        ClockPanel.qml
        DebugPanel.qml
        LowerMainPanel.qml
        PendulumPanel.qml
        SevenSegmentPanel.qml
        TimeSincePanel.qml
        UpperMainPanel.qml
)