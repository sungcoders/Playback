cmake_minimum_required(VERSION 3.15)
project(Sample)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    main.cpp 
)

add_executable(sample ${SOURCES})
