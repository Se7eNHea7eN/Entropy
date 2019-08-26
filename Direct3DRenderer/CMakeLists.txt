
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Direct3DRenderer)

project(${PROJECT_NAME})



file(GLOB SRC_FILES Src/*.cpp)
source_group("Sources" FILES ${SRC_FILES})
file(GLOB HEADERS_FILES Include/*.hpp)
source_group("Headers" FILES ${HEADERS_FILES})
add_library(${PROJECT_NAME} STATIC ${HEADERS_FILES} ${SRC_FILES})
include_directories(SYSTEM "Include")



include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")
