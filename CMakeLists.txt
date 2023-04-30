# cmake_minimum_required(VERSION 3.5)

# project(hello_qt_cmake LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(Qt6 COMPONENTS Core Gui Qml Quick QuickControls2 REQUIRED)

# add_executable(hello_qt_cmake main.cpp)

# target_link_libraries(hello_qt_cmake PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2)

# set(CMAKE_AUTORCC ON)
# qt6_add_resources(hello_qt_cmake "qml_resources"
# PREFIX
# "/"
# FILES
# "main.qml"
# )

# get_target_property(QT_BINARY_DIR Qt6::Core LOCATION)
# get_filename_component(QT_BINARY_DIR "${QT_BINARY_DIR}" DIRECTORY)

# add_custom_command(TARGET hello_qt_cmake POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy
# "${QT_BINARY_DIR}/Qt6Cored.dll"
# "${QT_BINARY_DIR}/Qt6Guid.dll"
# "${QT_BINARY_DIR}/Qt6Qmld.dll"
# "${QT_BINARY_DIR}/Qt6Widgetsd.dll"
# "${QT_BINARY_DIR}/Qt6Quickd.dll"
# "${QT_BINARY_DIR}/Qt6QuickWidgetsd.dll"
# "${QT_BINARY_DIR}/Qt6QuickControls2d.dll"
# $<TARGET_FILE_DIR:hello_qt_cmake>
# )

# set(QT_QML_QTQUICK_DIR "${QT_BINARY_DIR}/../qml/QtQuick")
# add_custom_command(TARGET hello_qt_cmake POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E make_directory
# $<TARGET_FILE_DIR:hello_qt_cmake>/QtQuick
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# "${QT_QML_QTQUICK_DIR}"
# $<TARGET_FILE_DIR:hello_qt_cmake>/QtQuick
# )

# set(QT_PLUGINS_DIR "${QT_BINARY_DIR}/../plugins/platforms")
# add_custom_command(TARGET hello_qt_cmake POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E make_directory
# $<TARGET_FILE_DIR:hello_qt_cmake>/platforms
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# "${QT_PLUGINS_DIR}"
# $<TARGET_FILE_DIR:hello_qt_cmake>/platforms
# )

# set(QT_QML_IMPORT_DIR "${QT_BINARY_DIR}/../qml")
# add_custom_command(TARGET hello_qt_cmake POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E make_directory
# $<TARGET_FILE_DIR:hello_qt_cmake>/qml
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# "${QT_QML_IMPORT_DIR}"
# $<TARGET_FILE_DIR:hello_qt_cmake>/qml
# )

# set_target_properties(hello_qt_cmake PROPERTIES
# VS_DEBUGGER_ENVIRONMENT "QML2_IMPORT_PATH=$<TARGET_FILE_DIR:hello_qt_cmake>/qml"
# )

cmake_minimum_required(VERSION 3.21)
project(micro_ui VERSION 0.1.0 LANGUAGES CXX)
set(namespace MrowrLib)

option(BUILD_EXAMPLE "Build the example target" ON)
option(BUILD_EXAMPLE_QT "Build the Qt example" ON) # turn on to enable Qt
option(BUILD_EXAMPLE_NANA "Build the Nana example" OFF)
option(BUILD_EXAMPLE_WX "Build the wxWidgets example" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endif()

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${namespace}::
    DESTINATION share/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

string(CONCAT config_cmake_in
    "@PACKAGE_INIT@\n"
    "include(CMakeFindDependencyMacro)\n"
    "find_dependency(_Log_ CONFIG)\n"
    "include(\"\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake\")\n"
)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.in" "${config_cmake_in}")

configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION share/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_PREFIX
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION share/${PROJECT_NAME}
)

if(BUILD_EXAMPLE)
    find_package(string_format CONFIG REQUIRED)

    add_executable(Example Example.cpp)
    target_compile_features(Example PRIVATE cxx_std_20)
    set_target_properties(Example PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(Example PRIVATE ${PROJECT_NAME} string_format::string_format)

    if(nana IN_LIST VCPKG_MANIFEST_FEATURES)
        find_package(unofficial-nana CONFIG REQUIRED)
        target_link_libraries(Example PRIVATE unofficial::nana::nana)
    endif()

    if(wxwidgets IN_LIST VCPKG_MANIFEST_FEATURES)
        find_package(wxWidgets CONFIG REQUIRED)
        target_link_libraries(Example PRIVATE wx::core wx::base)
    endif()
endif()
