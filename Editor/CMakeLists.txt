cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Editor)

project(${PROJECT_NAME} CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(SRC_FILES
    EntropyMainWindow.cpp
    main.cpp
    RenderWidget.cpp
    stdafx.cpp
)
source_group("Sources" FILES ${SRC_FILES})

set(HEADERS_FILES
  stdafx.h
  EntropyMainWindow.h
  RenderWidget.h
)
source_group("Headers" FILES ${HEADERS_FILES})

set(RESOURCES
  EntropyMainWindow.ui
  EntropyMainWindow.qrc
  Editor.rc
  )
source_group("Resources" FILES ${RESOURCES})

add_executable(${PROJECT_NAME}  
  ${SRC_FILES} 
  ${HEADERS_FILES}
  ${RESOURCES}
)
#set(CMAKE_PREFIX_PATH "Qt to path ") 
#qt5_use_modules(${PROJECT_NAME} Widgets)

#if(MSVC)
   #target_link_libraries(${PROJECT_NAME} qtmaind.lib Qt5Guid.lib Qt5Widgetsd.lib Qt5Cored.lib )
#endif(MSVC)


if (QTDIR OR DEFINED ENV{QTDIR} OR DEFINED ENV{QTDIR32} OR DEFINED ENV{QTDIR64})
 #Qt path set by user or env var
else()
set(QTDIR "" CACHE PATH "Path to Qt (e.g. C:/Qt/5.7/msvc2015_64)")
message(WARNING "QTDIR variable is missing.  Please set this variable to specify path to Qt (e.g. C:/Qt/5.7/msvc2015_64)")
endif()

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

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine")

target_link_libraries(
  ${PROJECT_NAME} 
	Engine
	Qt5::Core
  Qt5::Widgets
  Qt5::Gui
)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)