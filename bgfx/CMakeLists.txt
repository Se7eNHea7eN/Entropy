cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME bgfx)

project(${PROJECT_NAME})


file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c 3rdparty/meshoptimizer/src/*.cpp)
source_group("include" FILES ${project_headers})
source_group("src" FILES ${project_cpps})
set(all_files ${project_headers} ${project_cpps})

include_directories(SYSTEM "include")
include_directories(SYSTEM "3rdparty")
include_directories(SYSTEM "3rdparty/dxsdk/include")
include_directories(SYSTEM "3rdparty/khronos")

include_directories(SYSTEM "../bx/include/compat/msvc")
include_directories(SYSTEM "../bx/include")
include_directories(SYSTEM "../bimg/include")


add_library(${PROJECT_NAME} STATIC ${all_files})
