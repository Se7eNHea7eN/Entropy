cmake_minimum_required(VERSION 3.0.0)

project(Entropy)

add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(GLRenderer)
add_subdirectory(glew)
if(WIN32)
    add_subdirectory(Direct3DRenderer)
endif()

