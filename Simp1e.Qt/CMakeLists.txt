project(Simp1e.Qt)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES include/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

# Include Libraries
target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SvgWidgets
    function_pointer::function_pointer
)
