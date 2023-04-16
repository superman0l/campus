cmake_minimum_required(VERSION 3.5)

project(campus_local VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(../)

set(PROJECT_SOURCES
        affair.h
        map.h
        map.cpp
        person.h
        person.cpp
        basic.h
        basic.cpp
        timer.h
        main.cpp
        login.cpp
        login.h
        login.ui
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        regste.h
        regste.cpp
        regste.ui
        adminwindow.h
        adminwindow.cpp
        adminwindow.ui
        CustomTabStyle.h
        mapwin.h
        mapwin.cpp
        mapwin.ui
        affairwin.h
        affairwin.cpp
        affairwin.ui
        personwin.h
        personwin.cpp
        personwin.ui
        classwind.h
        classwind.cpp
        classwind.ui
        online_data.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(campus_local
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET campus_local APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(campus_local SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(campus_local
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(campus_local PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(campus_local PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS campus_local
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(campus_local)
endif()
