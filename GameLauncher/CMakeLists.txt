cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME GameLauncher)

project(${PROJECT_NAME})


#file(GLOB SRC_FILES Src/*.cpp)
source_group("Src" FILES ${SRC_FILES})


file(GLOB_RECURSE platform_headers Include/Platform/*.hpp Include/Platform/*.h)

if(WIN32)
    file(GLOB_RECURSE platform_cpps Src/Platform/Windows/*.cpp)
endif()
if(UNIX)
    file(GLOB_RECURSE platform_cpps Src/Platform/Linux/*.cpp)
endif()

if(APPLE)

endif()
set(all_files ${platform_headers} ${platform_cpps} ${SRC_FILES})

source_group_by_dir(all_files)

add_executable(${PROJECT_NAME} ${all_files})

include_directories(SYSTEM "Include")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")

target_link_libraries(${PROJECT_NAME} Engine)

if(WIN32)
    Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()
