cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME testScene)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Test")

include_directories(SYSTEM "Include")


include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/EntropyEngine/Include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/EntropyEngine/Include/Common")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Release")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Eigen")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include/compat/msvc")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx")
target_link_libraries(${PROJECT_NAME} bx)
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg")
target_link_libraries(${PROJECT_NAME} bimg)