project(Asteroids)

# Qt Creator needs header files to be added to the executable for them to show up and work correctly
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Setup application executable
qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION Asteroids.cpp ${HEADER_FILES})

# Include Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SvgWidgets)

# Add include/ folder of header files
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Required!
qt_finalize_executable(${PROJECT_NAME})

# target_link_libraries(${target_name} PRIVATE string_format::string_format _Log_::_Log_ fmt::fmt)
# target_link_libraries(${target_name} PRIVATE Simp1e Simp1e.ECS Simp1e.QT Simp1e.QT.ECS)
# set_target_properties(${target_name} PROPERTIES
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )
