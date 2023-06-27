project("Simp1e.QT.ECS")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE Simp1e Simp1e.ECS Simp1e.QT)

file(GLOB_RECURSE HEADER_FILES include/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} INTERFACE _Log_::_Log_ string_format::string_format)
