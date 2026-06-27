cmake_minimum_required(VERSION 4.2)
project(RayLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(raylib REQUIRED)

add_executable(RayLib main.cpp)

target_link_libraries(RayLib PRIVATE raylib)