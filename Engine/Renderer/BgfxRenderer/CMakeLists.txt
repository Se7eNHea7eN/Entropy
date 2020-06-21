cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME BgfxRenderer)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} STATIC ${all_files})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/Renderer")

include_directories(SYSTEM "Include")

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bgfx")
 target_link_libraries(${PROJECT_NAME} bgfx)

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bgfx/include")

 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/EntropyEngine/Include")

 
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include/compat/msvc")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx")
target_link_libraries(${PROJECT_NAME} bx)
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg")
target_link_libraries(${PROJECT_NAME} bimg)


include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bgfx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bgfx/3rdparty")




add_compile_definitions("__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;__STDC_CONSTANT_MACROS;")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Eigen")
