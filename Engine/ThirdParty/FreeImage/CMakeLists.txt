cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME FreeImage)

project(${PROJECT_NAME})


file(GLOB_RECURSE project_headers include/*.hpp include/*.h include/*.inl)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)

set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

include_directories(SYSTEM "/")
include_directories(SYSTEM "include")
include_directories(SYSTEM "../OpenEXR")
include_directories(SYSTEM "../OpenEXR/Imath")
include_directories(SYSTEM "../OpenEXR/Iex")
include_directories(SYSTEM "../OpenEXR/Half")
include_directories(SYSTEM "../LibJXR/jxrgluelib")
include_directories(SYSTEM "../LibJXR/image/sys")

add_library(${PROJECT_NAME} STATIC ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/ThirdParty")

