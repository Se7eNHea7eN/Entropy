cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME OpenEXR)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers *.hpp *.h )
file(GLOB_RECURSE project_cpps *.cpp *.c)

set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

include_directories(SYSTEM "/")
include_directories(SYSTEM "include")
include_directories(SYSTEM "Imath")
include_directories(SYSTEM "Iex")
include_directories(SYSTEM "Half")
include_directories(SYSTEM "IexMath")
include_directories(SYSTEM "IlmImf")
include_directories(SYSTEM "IlmThread")
include_directories(SYSTEM "Imath")
include_directories(SYSTEM "../ZLib")

add_library(${PROJECT_NAME} STATIC ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/ThirdParty")

