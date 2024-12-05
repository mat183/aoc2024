cmake_minimum_required(VERSION 3.5)

project(day5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES src/main.cpp)

add_executable(day5
    ${PROJECT_SOURCES}
)
