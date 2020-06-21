cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME test)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers include/*.hpp include/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp src/*.c)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_executable(${PROJECT_NAME} ${all_files})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Test")

include_directories(SYSTEM "Include")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GameLauncher/Include")
target_link_libraries(${PROJECT_NAME} GameLauncher)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include/Common")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Release")

if(WIN32)
    Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup ")
endif()

 
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/testScene/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/testScene")
target_link_libraries(${PROJECT_NAME} testScene)