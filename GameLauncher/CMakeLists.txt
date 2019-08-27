cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME GameLauncher)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers *.hpp *.h)
file(GLOB_RECURSE project_cpps *.cpp *.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_executable(${PROJECT_NAME} ${all_files})

include_directories(SYSTEM "Include")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")

target_link_libraries(${PROJECT_NAME} Engine)



