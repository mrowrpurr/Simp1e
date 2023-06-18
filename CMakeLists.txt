cmake_minimum_required(VERSION 3.5)

project(Simp1e-Projects VERSION 0.1 LANGUAGES CXX)

# message(FATAL_ERROR "QT_INSTALL_PREFIX: ${QT_INSTALL_PREFIX}")
# message(FATAL_ERROR "The system name is: ${CMAKE_SYSTEM_NAME}")
# message(FATAL_ERROR "Can we see the prefix path? ${CMAKE_PREFIX_PATH}")

add_definitions(-DSTRING_FORMAT_USE_FMT)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")
message(STATUS "CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}")

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    #message(FATAL_ERROR "We are here")
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
    # set(CMAKE_PREFIX_PATH "C:/Code/vcpkg/installed/arm64-android")
    set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1/android_arm64_v8a;C:/Code/vcpkg/installed/arm64-android")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1/wasm_multithread;C:/Code/vcpkg/installed/wasm32-emscripten")
else()
    set(CMAKE_PREFIX_PATH "C:/Code/vcpkg/installed/x64-windows-static")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(string_format CONFIG REQUIRED)
find_package(_Log_ CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory("Simp1e")
add_subdirectory("Simp1e.ECS")
add_subdirectory("Simp1e.ECS.Components")
add_subdirectory("Simp1e.QT")
add_subdirectory("Simp1e.QT.ECS")

# Update to support multiple or... just... hmm.
function(qt_project target_name main_cpp_file)
    qt_add_executable(${target_name} MANUAL_FINALIZATION "${main_cpp_file}" "Prototyping/Games/Side Scroller - Simpler/SideScroller.qrc")
    target_link_libraries(${target_name} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    get_filename_component(main_cpp_folder ${main_cpp_file} DIRECTORY)
    target_include_directories(${target_name} PRIVATE ${main_cpp_folder})
    target_link_libraries(${target_name} PRIVATE string_format::string_format _Log_::_Log_ fmt::fmt)
    target_link_libraries(${target_name} PRIVATE Simp1e Simp1e.ECS Simp1e.ECS.Components Simp1e.QT Simp1e.QT.ECS)
    set_target_properties(${target_name} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
    qt_finalize_executable(${target_name})
endfunction()

qt_project(Side_Scroller_Simpler "Prototyping/Games/Side Scroller - Simpler/Game - Simpler Side Scroller.cpp")
qt6_add_resources(Side_Scroller_Simpler "Prototyping/Games/Side Scroller - Simpler/SideScroller.qrc")
