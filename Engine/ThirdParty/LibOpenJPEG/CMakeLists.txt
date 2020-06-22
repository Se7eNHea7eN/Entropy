cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME LibOpenJPEG)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers *.hpp *.h )
file(GLOB_RECURSE project_cpps *.cpp *.c)

list(REMOVE_ITEM project_cpps "/jmemmac.c")

set(all_files ${project_headers} ${project_cpps})


source_group_by_dir(all_files)

include_directories(SYSTEM "/")

add_library(${PROJECT_NAME} STATIC ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/ThirdParty")

