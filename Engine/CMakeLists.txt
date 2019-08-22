# File generated at : 10:59:41, Thu 22 Aug 
# Converted Project : Engine.vcxproj
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

##################### Variables ############################
# Change if you want modify path or other values           #
############################################################

# Project
get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(DEPENDENCIES_DIR ${PROJECT_DIR}/dependencies)
set(PROJECT_NAME Engine)

# Dependencies
set(DIRECT3DRENDERER_DIR ../Direct3DRenderer)
set(GLRENDERER_DIR ../GLRenderer)

# Outputs
set(OUTPUT_DEBUG ${PROJECT_DIR}/bin)
set(OUTPUT_RELEASE ${PROJECT_DIR}/bin)

################# CMake Project ############################
# The main options of project                              #
############################################################

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

############## Artefacts Output ############################
# Defines outputs , depending BUILD TYPE                   #
############################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_DEBUG}")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_RELEASE}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_RELEASE}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${PROJECT_DIR}/${OUTPUT_RELEASE}")
endif()

# Messages
message("${PROJECT_NAME}: MAIN PROJECT: ${CMAKE_PROJECT_NAME}")
message("${PROJECT_NAME}: CURR PROJECT: ${CMAKE_CURRENT_SOURCE_DIR}")
message("${PROJECT_NAME}: CURR BIN DIR: ${CMAKE_CURRENT_BINARY_DIR}")

################## Dependencies ############################
# Add Dependencies to project                              #
############################################################

# Dependencies: disable BUILD_DEPENDS to link with libraries already built
option(BUILD_DEPENDS "Build other CMake project." ON)

if(BUILD_DEPENDS)
  if(NOT TARGET Direct3DRenderer)
    add_subdirectory("${DIRECT3DRENDERER_DIR}" ${CMAKE_BINARY_DIR}/Direct3DRenderer)
  endif()
  if(NOT TARGET GLRenderer)
    add_subdirectory("${GLRENDERER_DIR}" ${CMAKE_BINARY_DIR}/GLRenderer)
  endif()
else()
   link_directories(${DEPENDENCIES_DIR}/Direct3DRenderer)
   link_directories(${DEPENDENCIES_DIR}/GLRenderer)
endif()

############### Files & Targets ############################
# Files of project and target to build                     #
############################################################

# Source Files
set(SRC_FILES
    Private/EntropyCore.cpp
    Private/RendererFactory.cpp
    Private/Scene.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Header Files
set(HEADERS_FILES
    Public/EntropyCore.h
    Public/Scene.h
)
source_group("Headers" FILES ${HEADERS_FILES})

# Add executable to build.


######################### Flags ############################
# Defines Flags for Windows and Linux                      #
############################################################

########### Link & Dependencies ############################
# Add project dependencies and Link to project             #
############################################################

# Project dependencies
#add_dependencies(Direct3DRenderer GLRenderer)

# Link with other dependencies.
#target_link_libraries(Direct3DRenderer GLRenderer )
