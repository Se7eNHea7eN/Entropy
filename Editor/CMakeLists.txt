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

Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")
