cmake_minimum_required(VERSION 3.23)
project(MandelbrotZoom)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
set(CMAKE_CXX_COMPILER "clang++")
add_executable(MandelbrotZoom main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

