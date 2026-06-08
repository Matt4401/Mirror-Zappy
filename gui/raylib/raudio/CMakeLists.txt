add_library(zappy_gui_raylib_raudio INTERFACE)
target_sources(zappy_gui_raylib_raudio INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Audio.hpp
                Sound.hpp
                Wave.hpp
)
target_include_directories(zappy_gui_raylib_raudio INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_raudio INTERFACE
        zappy_gui_raylib_external
)
