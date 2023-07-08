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

# Find Qt packages
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SvgWidgets)

# Find vcpkg packages
find_package(fmt CONFIG REQUIRED)
find_package(string_format CONFIG REQUIRED)
find_package(_Log_ CONFIG REQUIRED)
find_package(void_pointer CONFIG REQUIRED)
find_package(function_pointer CONFIG REQUIRED)

add_subdirectory("Simp1e.Core")
add_subdirectory("Simp1e.ECS.Core")

add_subdirectory("Experimentation")
