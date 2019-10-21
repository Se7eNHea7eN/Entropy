cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME testScene)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} ${all_files})

include_directories(SYSTEM "Include")


include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include/Common")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Release")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Eigen")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx/include/compat/msvc")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx")
target_link_libraries(${PROJECT_NAME} bx)
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bimg/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bimg")
target_link_libraries(${PROJECT_NAME} bimg)