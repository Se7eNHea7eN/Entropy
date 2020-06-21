cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME bx)

project(${PROJECT_NAME})


file(GLOB_RECURSE project_headers include/*.hpp include/*.h include/*.inl)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)

set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

include_directories(SYSTEM "include")
include_directories(SYSTEM "include/compat/msvc")
include_directories(SYSTEM "3rdparty")

add_library(${PROJECT_NAME} STATIC ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/ThirdParty")

add_compile_definitions("__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;__STDC_CONSTANT_MACROS;")
