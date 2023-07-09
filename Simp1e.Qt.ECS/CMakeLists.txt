project(Simp1e.Qt.ECS)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES include/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
    Simp1e.Qt
    Simp1e.ECS.Core
    Simp1e.ECS.Components.Common
    Simp1e.ECS.Components.GUI
)
