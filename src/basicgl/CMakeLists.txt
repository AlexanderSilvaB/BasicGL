cmake_minimum_required(VERSION 2.8)
project(basicgl)

option(IMAGE_CAPTURE "Add support to capture images from file/devices" ON)

include(GNUInstallDirs)

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/libs/glew-2.1.0")
    find_package(GLEW)
    if (GLEW_FOUND)
        set (BASIC_GL_FOUND TRUE)
    else()
        set (BASIC_GL_FOUND FALSE)
    endif()
else()
    find_package(OpenGL)
    if (OpenGL_FOUND)
        set (BASIC_GL_FOUND TRUE)
    else()
        set (BASIC_GL_FOUND FALSE)
    endif()
endif()

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/libs/freeglut")
endif()
find_package(GLUT)
if (NOT GLUT_FOUND)
    set (BASIC_GL_FOUND FALSE)
endif()

if (BASIC_GL_FOUND)
    if(IMAGE_CAPTURE)
        message("Trying to use image capture")
        find_package(OpenCV)
        if(OpenCV_FOUND)
            message("Using OpenCV as capture library")
            add_definitions(-DIMAGE_CAPTURE)
        endif(OpenCV_FOUND)
    endif(IMAGE_CAPTURE)

    if(WIN32)
        include_directories( ${GLEW_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
    else()
        include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
    endif()

    include_directories(external)

    set(SRC win_unistd.c
            Camera.cpp
            LightSource.cpp
            Point.cpp
            Texture.cpp
            OBJ.cpp
            Font.cpp
            Element.cpp
            Serie.cpp
            Plot.cpp
            Window.cpp
            Manager.cpp 
            external/jo_mpeg/jo_mpeg.cpp)

    add_library(${PROJECT_NAME} ${SRC})

    if(WIN32)
        target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} )
    else()
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} )
    endif()

    target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARY} )
    if(IMAGE_CAPTURE AND OpenCV_FOUND)
        target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/../cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(DIRECTORY ${PROJECT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h*" PATTERN "basicgl/external" EXCLUDE)
    install (TARGETS ${PROJECT_NAME} DESTINATION lib)
else()
    message("Could not build basicgl. OpenGL, GLUT or GLEW missing")
endif()