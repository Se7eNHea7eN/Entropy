cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Engine)

project(${PROJECT_NAME})

file(GLOB_RECURSE project_headers *.hpp *.h)
file(GLOB_RECURSE project_cpps *.cpp *.c *.cc)
file(GLOB_RECURSE shaders *.sc *.fs *.vs)

set(all_files ${project_headers} ${project_cpps} ${shaders})

source_group_by_dir(all_files)

add_library(${PROJECT_NAME} STATIC ${all_files})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine")


include_directories(SYSTEM "Include")

if(WIN32)
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/Direct3DRenderer")
    include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/Direct3DRenderer/Include")
    target_link_libraries(${PROJECT_NAME} Direct3DRenderer)
endif()

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/GLRenderer")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/GLRenderer/Include")
target_link_libraries(${PROJECT_NAME} GLRenderer)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/Eigen")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/BgfxRenderer")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Renderer/BgfxRenderer/Include")
target_link_libraries(${PROJECT_NAME} BgfxRenderer)
 
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx/include/compat/msvc")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bx")
target_link_libraries(${PROJECT_NAME} bx)
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/ThirdParty/bimg/3rdparty")
target_link_libraries(${PROJECT_NAME} bimg)

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