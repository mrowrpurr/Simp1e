project(Asteroids)

# Setup application executable
qt_add_executable(
    ${PROJECT_NAME}
    Asteroids.cpp
    Asteroids.qrc
)

# Qt Creator needs header files added as target sources
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

# Include Qt
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SvgWidgets
    Qt${QT_VERSION_MAJOR}::Sensors
)

# Include required Simp1e libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Simp1e.Core
    Simp1e.ECS.Local
    Simp1e.Qt.ECS
    fmt::fmt-header-only
    string_format::string_format
)

# Add include/ folder of header files
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# set_target_properties(${target_name} PROPERTIES
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )
