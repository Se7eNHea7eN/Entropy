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
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

#add_test(NAME Game.exe COMMAND Game.exe WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})


set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Debug")

#add_compile_definitions("__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;__STDC_CONSTANT_MACROS;_DEBUG;WIN32;_WIN32;_HAS_EXCEPTIONS=0;_SCL_SECURE=0;_SECURE_SCL=0;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;")