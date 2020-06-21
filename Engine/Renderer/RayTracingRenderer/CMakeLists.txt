cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME RayTracingRenderer)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} STATIC ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/Renderer")

include_directories(SYSTEM "Include")


include_directories("Private/glew/include/")


 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/glew")
 #target_link_libraries(${PROJECT_NAME} glew)
 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/glew/include")


 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/EntropyEngine/Include")


include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Eigen")


target_link_libraries(${PROJECT_NAME} debug   ${CMAKE_CURRENT_SOURCE_DIR}/../glew/libs/glew32d.lib)
target_link_libraries(${PROJECT_NAME} optimized ${CMAKE_CURRENT_SOURCE_DIR}/../glew/libs/glew32.lib)
target_link_libraries(${PROJECT_NAME} opengl32.lib )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS_RELEASE "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")