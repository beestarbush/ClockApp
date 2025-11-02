qt_add_library(MainLib STATIC)

set_target_properties(MainLib PROPERTIES AUTOMOC ON)
target_link_libraries(MainLib PRIVATE Qt6::Quick)

qt_add_qml_module(MainLib
    URI Main
    VERSION 1.0
    QML_FILES
        Main.qml
        Content.qml
    DEPENDENCIES
        Components/1.0
        Panels/1.0
        Bee/1.0
)