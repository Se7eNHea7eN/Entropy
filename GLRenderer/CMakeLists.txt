cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME GLRenderer)

project(${PROJECT_NAME})


file(GLOB SRC_FILES Src/*.cpp)
source_group("Sources" FILES ${SRC_FILES})
file(GLOB HEADERS_FILES Include/*.hpp)
source_group("Headers" FILES ${HEADERS_FILES})
add_library(${PROJECT_NAME} STATIC ${HEADERS_FILES} ${SRC_FILES})
include_directories(SYSTEM "Include")


#GLEW
include_directories("Private/glew/include/")
set(GLEW_HEADERS_FILES
    Private/glew/include/GL/glew.h
    Private/glew/include/GL/wglew.h
)
set(GLEW_SRC_FILES
    Private/glew/src/glew.c
)
source_group("GLEW" FILES ${GLEW_HEADERS_FILES} ${GLEW_SRC_FILES})
#!GLEW

 #${GLEW_HEADERS_FILES} 
 #${GLEW_SRC_FILES}
 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/glew")
 target_link_libraries(${PROJECT_NAME} glew)
 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/glew/include")


 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")

