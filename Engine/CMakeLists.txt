cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Engine)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers *.hpp *.h)
file(GLOB_RECURSE project_cpps *.cpp *.c *.cc)
set(all_files ${project_headers} ${project_cpps})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} STATIC ${all_files})

include_directories(SYSTEM "Include")

if(WIN32)
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Direct3DRenderer")
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Direct3DRenderer/Include")
    target_link_libraries(${PROJECT_NAME} Direct3DRenderer)
endif()

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GLRenderer")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GLRenderer/Include")
target_link_libraries(${PROJECT_NAME} GLRenderer)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Eigen")



include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/BgfxRenderer")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/BgfxRenderer/Include")
target_link_libraries(${PROJECT_NAME} BgfxRenderer)


#set (CMAKE_C_COMPILER               "clang-cl")
#set (CMAKE_C_FLAGS                  "-Wall")
#set (CMAKE_C_FLAGS_DEBUG            "/Debug")
#set (CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
#set (CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG")
#set (CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 /Debug")
#set (CMAKE_C_STANDARD 11)
#set (CMAKE_CXX_COMPILER             "clang-cl")
#set (CMAKE_CXX_FLAGS                "-Wall -Xclang -std=gnu++14")
#set (CMAKE_CXX_FLAGS_DEBUG          "/Debug")
#set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 /Debug")