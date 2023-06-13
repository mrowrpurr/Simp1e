cmake_minimum_required(VERSION 3.21)

project(Simp1e LANGUAGES CXX)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/android-build)

