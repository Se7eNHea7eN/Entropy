cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME IblPrecompute)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_executable(${PROJECT_NAME} ${all_files})


include_directories(SYSTEM "Include")


include_directories("Private/glew/include/")


include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Eigen")

 
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx/include/compat/msvc")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bx")
target_link_libraries(${PROJECT_NAME} bx)
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bimg/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/bimg")
target_link_libraries(${PROJECT_NAME} bimg)


 include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/glew/include")

 if(WIN32)
    Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup ")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Release")

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../glew/libs/glew32.lib)
 target_link_libraries(${PROJECT_NAME} opengl32.lib )
