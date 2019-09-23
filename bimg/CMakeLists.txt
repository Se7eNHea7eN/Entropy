cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME bimg)

project(${PROJECT_NAME})


file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps
src/*.cpp src/*.c
3rdparty/astc-codec/src/decoder/*.cc
3rdparty/astc-codec/src/decoder/*.h
3rdparty/miniz/*.hpp
3rdparty/miniz/*.cpp
)
set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)
include_directories(SYSTEM "include")
include_directories(SYSTEM "3rdparty")
include_directories(SYSTEM "3rdparty/tinyexr")
include_directories(SYSTEM "3rdparty/iqa/include")
include_directories(SYSTEM "3rdparty/astc-codec")
include_directories(SYSTEM "3rdparty/astc-codec/include")

include_directories(SYSTEM "../bx/include")
include_directories(SYSTEM "../bx/include/compat/msvc")

add_library(${PROJECT_NAME} STATIC ${all_files})
