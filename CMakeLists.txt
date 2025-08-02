cmake_minimum_required(VERSION 3.16)

project(TextEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TextEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCE_FILES}
        resources.qrc
    )
else()
    if(ANDROID)
        add_library(TextEditor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TextEditor
            ${PROJECT_SOURCES}
            ${RESOURCE_FILES}
        )
    endif()
endif()

if(WIN32)
    set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/exeFileIcon.ico")

    if(EXISTS "${APP_ICON}")
        message(STATUS "Found icon file: ${APP_ICON}")

        set(RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")
        file(WRITE ${RC_FILE} "IDI_ICON1 ICON \"${APP_ICON}\"")

        target_sources(TextEditor PRIVATE ${RC_FILE})

        set_target_properties(TextEditor PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "/MANIFEST:NO"
        )
    else()
        message(WARNING "Icon file not found: ${APP_ICON}")
    endif()
endif()

target_include_directories(TextEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TextEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)



if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TextEditor)
endif()
set_target_properties(TextEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TextEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TextEditor)
endif()
