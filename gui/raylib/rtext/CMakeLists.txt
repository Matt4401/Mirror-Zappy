add_library(zappy_gui_raylib_rtext INTERFACE)
target_sources(zappy_gui_raylib_rtext INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Font.hpp
                Text.hpp
)
target_include_directories(zappy_gui_raylib_rtext INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rtext INTERFACE
        zappy_gui_raylib_external
)
