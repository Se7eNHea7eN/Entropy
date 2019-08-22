# File generated at : 10:26:29, Thu 22 Aug 
# Converted Project : E:/WorkSpace/Entropy/Editor/Editor.vcxproj
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

##################### Variables ############################
# Change if you want modify path or other values           #
############################################################

# Project
get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(DEPENDENCIES_DIR ${PROJECT_DIR}/dependencies)
set(PROJECT_NAME Editor)

# Dependencies
set(ENGINE_DIR ../Engine)

# Outputs
set(OUTPUT_DEBUG ./)
set(OUTPUT_RELEASE ./)

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
  if(NOT TARGET Engine)
    add_subdirectory("${ENGINE_DIR}" ${CMAKE_BINARY_DIR}/Engine)
  endif()
else()
   link_directories(${DEPENDENCIES_DIR}/Engine)
endif()

############### Files & Targets ############################
# Files of project and target to build                     #
############################################################

# Source Files
set(SRC_FILES
    Private/EntropyMainWindow.cpp
    Private/main.cpp
    Private/RenderWidget.cpp
    Private/stdafx.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Header Files
set(HEADERS_FILES
    Private/stdafx.h
)
source_group("Headers" FILES ${HEADERS_FILES})

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${SRC_FILES} ${HEADERS_FILES}
)

######################### Flags ############################
# Defines Flags for Windows and Linux                      #
############################################################

if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

# Preprocessor definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
   -DUNICODE 
   -D_UNICODE 
   -D_ENABLE_EXTENDED_ALIGNED_STORAGE 
   -DWIN64 
   -DQT_GUI_LIB 
   -DQT_WIDGETS_LIB 
   -DQT_CORE_LIB 
    )
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE  /MDd /Od /Zi /EHsc)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
   -DUNICODE 
   -D_UNICODE 
   -D_ENABLE_EXTENDED_ALIGNED_STORAGE 
   -DWIN64 
   -DQT_NO_DEBUG 
   -DNDEBUG 
   -DQT_GUI_LIB 
   -DQT_WIDGETS_LIB 
   -DQT_CORE_LIB 
    )
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE  /EHsc)
    endif()
endif()

########### Link & Dependencies ############################
# Add project dependencies and Link to project             #
############################################################

# Project dependencies
add_dependencies(${PROJECT_NAME} Engine)

# Link with other dependencies.
target_link_libraries(${PROJECT_NAME} Engine )
if(MSVC)
   #target_link_libraries(${PROJECT_NAME} qtmaind.lib Qt5Guid.lib Qt5Widgetsd.lib Qt5Cored.lib )
endif(MSVC)


#if (QTDIR OR DEFINED ENV{QTDIR} OR DEFINED ENV{QTDIR32} OR DEFINED ENV{QTDIR64})
# Qt path set by user or env var
#else()
#set(QTDIR "" CACHE PATH "Path to Qt (e.g. C:/Qt/5.7/msvc2015_64)")
#message(WARNING "QTDIR variable is missing.  Please set this variable to specify path to Qt (e.g. C:/Qt/5.7/msvc2015_64)")
#endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(_lib_suffix 64)
else()
set(_lib_suffix 32)
endif()

if(DEFINED QTDIR${_lib_suffix})
list(APPEND CMAKE_PREFIX_PATH "${QTDIR${_lib_suffix}}")
elseif(DEFINED QTDIR)
list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR${_lib_suffix}})
list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR${_lib_suffix}}")
elseif(DEFINED ENV{QTDIR})
list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()


find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5GUI REQUIRED)

target_link_libraries(
	#Engine
	#Qt5::Core
	Qt5::Widgets
)
