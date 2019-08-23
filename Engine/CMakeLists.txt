cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Engine)

project(${PROJECT_NAME})

set(SRC_FILES
    Private/EntropyCore.cpp
    Private/RendererFactory.cpp
    Private/Scene.cpp
)
source_group("Sources" FILES ${SRC_FILES})

set(HEADERS_FILES
    Public/EntropyCore.h
    Public/Scene.h
)
source_group("Headers" FILES ${HEADERS_FILES})

add_library(${PROJECT_NAME} STATIC ${HEADERS_FILES} ${SRC_FILES})

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Direct3DRenderer")
target_link_libraries(${PROJECT_NAME} Direct3DRenderer)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/GLRenderer")
target_link_libraries(${PROJECT_NAME} GLRenderer)
