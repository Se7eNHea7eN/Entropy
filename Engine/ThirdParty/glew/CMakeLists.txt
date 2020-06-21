
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)


set(PROJECT_NAME glew)

project(${PROJECT_NAME} C)

add_compile_definitions("GLEW_BUILD")

# Header Files
set(HEADERS_FILES
    include/GL/glew.h
    include/GL/wglew.h
)
source_group("Headers" FILES ${HEADERS_FILES})
include_directories("include/")
# Add library to build.
#add_library(${PROJECT_NAME} STATIC  ${HEADERS_FILES})

########### Link & Dependencies ############################
# Add project dependencies and Link to project             #
############################################################

#if(MSVC)
 # target_link_libraries(${PROJECT_NAME} opengl32.lib )
#endif(MSVC)
 #target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew32.lib)