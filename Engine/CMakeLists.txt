cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Engine)

project(${PROJECT_NAME})


file(GLOB_RECURSE SRC_FILES Src/*.cpp Src/*.c)
source_group("Sources" FILES ${SRC_FILES})
file(GLOB_RECURSE HEADERS_FILES Include/*.hpp Include/*.h)
source_group("Headers" FILES ${HEADERS_FILES})
add_library(${PROJECT_NAME} STATIC ${HEADERS_FILES} ${SRC_FILES})
include_directories(SYSTEM "Include")

if(WIN32)
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Direct3DRenderer")
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Direct3DRenderer/Include")
    target_link_libraries(${PROJECT_NAME} Direct3DRenderer)
endif()

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GLRenderer")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GLRenderer/Include")
target_link_libraries(${PROJECT_NAME} GLRenderer)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Eigen")


