cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME bimg)

project(${PROJECT_NAME})


file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c   3rdparty/astc-codec/src/decoder/*.cc 3rdparty/astc-codec/src/decoder/*.h)
set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)
include_directories(SYSTEM "include")
include_directories(SYSTEM "3rdparty")
include_directories(SYSTEM "3rdparty/iqa/include")
include_directories(SYSTEM "3rdparty/astc-codec")
include_directories(SYSTEM "3rdparty/astc-codec/include")

include_directories(SYSTEM "../bx/include")
include_directories(SYSTEM "../bx/include/compat/msvc")

add_library(${PROJECT_NAME} STATIC ${all_files})
#add_compile_definitions("__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;__STDC_CONSTANT_MACROS;_DEBUG;WIN32;_WIN32;_HAS_EXCEPTIONS=0;_SCL_SECURE=0;_SECURE_SCL=0;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;BX_CONFIG_DEBUG=1;")
