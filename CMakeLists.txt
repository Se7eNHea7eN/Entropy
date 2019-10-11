cmake_minimum_required(VERSION 3.0.0)

project(Entropy)


macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)


add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(GLRenderer)
add_subdirectory(glew)
add_subdirectory(GameLauncher)

if(WIN32)
    add_subdirectory(Direct3DRenderer)
endif()

add_subdirectory(bgfx)
add_subdirectory(bx)
add_subdirectory(bimg)
add_subdirectory(BgfxRenderer)
add_subdirectory(test)

add_subdirectory(IblPrecompute)

add_subdirectory(glfw)

