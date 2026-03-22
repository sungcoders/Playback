
cmake_minimum_required(VERSION 3.15)

project(SLVProduction CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Playback)
add_subdirectory(Sample)
add_subdirectory(Utils)