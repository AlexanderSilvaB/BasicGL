cmake_minimum_required(VERSION 2.8)

project(examples)

include_directories(..)

add_executable(triangle triangle.cpp)
target_link_libraries(triangle basicgl)

add_executable(heart heart.cpp)
target_link_libraries(heart basicgl)