cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME Editor)

project(${PROJECT_NAME} CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

FILE(GLOB SRC_FILES *.cpp *.c)
source_group("Sources" FILES ${SRC_FILES})

file(GLOB HEADERS_FILES *.hpp *.h)
source_group("Headers" FILES ${HEADERS_FILES})

file(GLOB RESOURCES *.ui *.rc *.qrc)
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
include_directories(SYSTEM "Include")


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../RunDir/)

Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Engine/Include")

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}Release")
 
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/testScene/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/testScene")
target_link_libraries(${PROJECT_NAME} testScene)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Editor")
