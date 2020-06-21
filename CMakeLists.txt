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


#add_subdirectory(Editor)
add_subdirectory(Engine/EntropyEngine)
add_subdirectory(Engine/Renderer/GLRenderer)
if(WIN32)
    add_subdirectory(Engine/Renderer/Direct3DRenderer)
    add_subdirectory(Engine/Renderer/RayTracingRenderer)
endif()

add_subdirectory(Engine/Renderer/BgfxRenderer)

add_subdirectory(Engine/Tools/IblPrecompute)

add_subdirectory(Engine/ThirdParty/glew)
add_subdirectory(Engine/ThirdParty/bgfx)
add_subdirectory(Engine/ThirdParty/bx)
add_subdirectory(Engine/ThirdParty/bimg)
add_subdirectory(Engine/ThirdParty/glfw)


add_subdirectory(test/GameLauncher)

add_subdirectory(test/test)
add_subdirectory(test/testScene)





