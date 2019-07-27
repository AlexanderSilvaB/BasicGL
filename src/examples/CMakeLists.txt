cmake_minimum_required(VERSION 2.8)

project(examples)

include_directories(..)

add_executable(triangle triangle.cpp)
target_link_libraries(triangle basicgl)

add_executable(heart heart.cpp)
target_link_libraries(heart basicgl)

add_executable(snowman snowman.cpp)
target_link_libraries(snowman basicgl)

add_executable(teapot teapot.cpp)
target_link_libraries(teapot basicgl)

add_executable(plot plot.cpp)
target_link_libraries(plot basicgl)

add_executable(robot robot.cpp)
target_link_libraries(robot basicgl)

add_executable(text text.cpp)
target_link_libraries(text basicgl)