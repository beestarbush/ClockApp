cmake_minimum_required(VERSION 3.16)

project(ClockApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(clockapp
    main.cpp
    git_version.h
    hal/HAL.cpp
    hal/HAL.h
    hal/Network.cpp
    hal/Network.h
    hal/Screen.cpp
    hal/Screen.h
    hal/Storage.cpp
    hal/Storage.h
    hal/System.cpp
    hal/System.h
    hal/Temperature.cpp
    hal/Temperature.h
    qmlcomponents/QmlUtils.cpp
    qmlcomponents/QmlUtils.h
    qmlcomponents/RoundAnimatedImage.cpp
    qmlcomponents/RoundAnimatedImage.h
    services/MediaManager.cpp
    services/MediaManager.h
    services/DateTime.cpp
    services/DateTime.h
    services/Notification.cpp
    services/Notification.h
    services/NotificationManager.cpp
    services/NotificationManager.h
    services/QmlInterface.cpp
    services/QmlInterface.h
    services/Services.cpp
    services/Services.h
    services/Version.cpp
    services/Version.h
    applications/Applications.cpp
    applications/Applications.h
    applications/BirthdayTimer.cpp
    applications/BirthdayTimer.h
    applications/Clock.cpp
    applications/Clock.h
    applications/MarriedTimer.cpp
    applications/MarriedTimer.h
    applications/TimeElapsedTimer.cpp
    applications/TimeElapsedTimer.h
    applications/CountdownTimer.cpp
    applications/CountdownTimer.h
    applications/Setup.cpp
    applications/Setup.h
)

qt_add_qml_module(clockapp
    URI ClockApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

# This only contains default media (when lookup on filesystem is not possible)
qt_add_resources(clockapp "media"
    PREFIX "/"
    FILES
        "media/test.gif"
)

if (CMAKE_CXX_COMPILER MATCHES "aarch64-poky-linux")
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
