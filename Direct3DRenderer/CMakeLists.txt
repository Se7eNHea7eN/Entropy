
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Direct3DRenderer)

project(${PROJECT_NAME})

set(SRC_FILES
    Private/Direct3DRenderer.cpp
    Private/stdafx.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Header Files
set(HEADERS_FILES
    Public/Direct3DRenderer.h
)
source_group("Headers" FILES ${HEADERS_FILES})



add_library(${PROJECT_NAME} STATIC ${HEADERS_FILES} ${SRC_FILES})



include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")
