cmake_minimum_required(VERSION 2.8)

project(examples)

if(TARGET basicgl)
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

    add_executable(paint paint.cpp)
    target_link_libraries(paint basicgl)

    add_executable(texture texture.cpp)
    target_link_libraries(texture basicgl)

    add_executable(obj obj.cpp)
    target_link_libraries(obj basicgl)

    add_executable(viewer viewer.cpp)
    target_link_libraries(viewer basicgl)

    add_executable(webcam webcam.cpp)
    target_link_libraries(webcam basicgl)

    add_executable(light light.cpp)
    target_link_libraries(light basicgl)
endif()