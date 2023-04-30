cmake_minimum_required(VERSION 3.5)

# set(CMAKE_PREFIX_PATH "C:/QtFull/6.4.3/msvc2019_64/lib/cmake")
project(hello_qt_cmake LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(hello_qt_cmake
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(hello_qt_cmake PRIVATE Qt6::Widgets)

# Add this line to set the appropriate entry point for a GUI application
# target_compile_definitions(hello_qt_cmake PRIVATE _CONSOLE)

# Copy required Qt DLLs to the build directory
get_target_property(QT_BINARY_DIR Qt6::Core LOCATION)
get_filename_component(QT_BINARY_DIR "${QT_BINARY_DIR}" DIRECTORY)

add_custom_command(TARGET hello_qt_cmake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${QT_BINARY_DIR}/Qt6Core.dll"
    "${QT_BINARY_DIR}/Qt6Gui.dll"
    "${QT_BINARY_DIR}/Qt6Widgets.dll"
    "${QT_BINARY_DIR}/Qt6Cored.dll"
    "${QT_BINARY_DIR}/Qt6Guid.dll"
    "${QT_BINARY_DIR}/Qt6Widgetsd.dll"
    $<TARGET_FILE_DIR:hello_qt_cmake>
)

# Copy the required platform plugins to the build directory
set(QT_PLUGINS_DIR "${QT_BINARY_DIR}/../plugins/platforms")
add_custom_command(TARGET hello_qt_cmake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:hello_qt_cmake>/platforms
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${QT_PLUGINS_DIR}"
    $<TARGET_FILE_DIR:hello_qt_cmake>/platforms
)
