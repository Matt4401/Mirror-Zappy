find_package(raylib CONFIG QUIET)

if (TARGET raylib)
        add_library(zappy_gui_raylib_external INTERFACE)
        target_link_libraries(zappy_gui_raylib_external INTERFACE raylib)
elseif (TARGET raylib::raylib)
        add_library(zappy_gui_raylib_external INTERFACE)
        target_link_libraries(zappy_gui_raylib_external INTERFACE raylib::raylib)
else()
        find_package(PkgConfig QUIET)
        if (PkgConfig_FOUND)
                pkg_check_modules(RAYLIB QUIET IMPORTED_TARGET raylib)
        endif()

        if (TARGET PkgConfig::RAYLIB)
                add_library(zappy_gui_raylib_external INTERFACE)
                target_link_libraries(zappy_gui_raylib_external INTERFACE PkgConfig::RAYLIB)
        else()
                message(FATAL_ERROR "raylib not found. Install raylib development files or set raylib_DIR, CMAKE_PREFIX_PATH, or PKG_CONFIG_PATH.")
        endif()
endif()

add_subdirectory(raudio)
add_subdirectory(rcore)
add_subdirectory(rmath)
add_subdirectory(rmodels)
add_subdirectory(rtext)
add_subdirectory(rtextures)

add_library(zappy_gui_raylib INTERFACE)
target_sources(zappy_gui_raylib INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(zappy_gui_raylib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(zappy_gui_raylib INTERFACE
        zappy_gui_raylib_raudio
        zappy_gui_raylib_rcore
        zappy_gui_raylib_rmath
        zappy_gui_raylib_rmodels
        zappy_gui_raylib_rtext
        zappy_gui_raylib_rtextures
)
