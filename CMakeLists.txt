cmake_minimum_required(VERSION 3.13)

project(libkorra VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PROJECT_SOURCES_GUI CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS_GUI CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.hpp")
file(GLOB_RECURSE PROJECT_UI CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.ui")


##### qt stuff #####
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled
        MANUAL_FINALIZATION
        main.cpp
        Makefile
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES_GUI}
        ${PROJECT_HEADERS_GUI}
        ${PROJECT_UI}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            main.cpp
            Makefile
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_SOURCES_GUI}
            ${PROJECT_HEADERS_GUI}
            ${PROJECT_UI}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            main.cpp
            Makefile
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_SOURCES_GUI}
            ${PROJECT_HEADERS_GUI}
            ${PROJECT_UI}
        )
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled)
endif()
##### /qt stuff #####

target_include_directories(${PROJECT_NAME} PRIVATE include GUI)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets m z)
