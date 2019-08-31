cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME BgfxRenderer)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} STATIC ${all_files})


include_directories(SYSTEM "Include")

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bgfx")
 target_link_libraries(${PROJECT_NAME} bgfx)

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bgfx/include")

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")

 
 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx")
 target_link_libraries(${PROJECT_NAME} bx)
 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bimg")
 target_link_libraries(${PROJECT_NAME} bimg)
 

 include_directories(SYSTEM "../bgfx/iinclude")
include_directories(SYSTEM "../bgfx/3rdparty")
include_directories(SYSTEM "../bgfx/3rdparty/dxsdk/include")
include_directories(SYSTEM "../bgfx/3rdparty/khronos")

include_directories(SYSTEM "../bx/include/compat/msvc")
include_directories(SYSTEM "../bx/include")
include_directories(SYSTEM "../bimg/include")

#add_compile_definitions("__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;__STDC_CONSTANT_MACROS;_DEBUG;WIN32;_WIN32;_HAS_EXCEPTIONS=0;_HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0;_SCL_SECURE=0;_SECURE_SCL=0;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;BX_CONFIG_DEBUG=1;")