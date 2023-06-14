cmake_minimum_required(VERSION 3.5)

project(Simp1e-Projects VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

function(qt_project target_name main_cpp_file)
    qt_add_executable(${target_name} MANUAL_FINALIZATION "${main_cpp_file}")
    target_link_libraries(${target_name} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    get_filename_component(main_cpp_folder ${main_cpp_file} DIRECTORY)
    target_include_directories(${target_name} PRIVATE ${main_cpp_folder})
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
