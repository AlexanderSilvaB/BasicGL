cmake_minimum_required(VERSION 2.8)

project(basicgl)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(SRC BasicGL.cpp)

add_library(basicgl ${SRC})
target_link_libraries(basicgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )