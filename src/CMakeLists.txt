cmake_minimum_required(VERSION 3.16)

project(ClockApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(clockapp
    main.cpp
    FunkyClass.cpp
    FunkyClass.h
)

qt_add_qml_module(clockapp
    URI ClockApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

if (CMAKE_CXX_COMPILER MATCHES "aarch64-oe-linux")
    set(PLATFORM "target")
    add_definitions(-DPLATFORM_IS_TARGET)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM "host")
    add_definitions(-DPLATFORM_IS_HOST)
endif()

add_subdirectory(qml)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(clockapp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appClockApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(clockapp
    PRIVATE Qt6::Quick ComponentLibplugin PanelLibplugin
)

include(GNUInstallDirs)
install(TARGETS clockapp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
