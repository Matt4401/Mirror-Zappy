add_library(zappy_gui_graphics_scene
        Map.cpp
        SkyBackground.cpp
        Tile3D.cpp
)
target_sources(zappy_gui_graphics_scene PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Map.hpp
        SkyBackground.hpp
        Tile3D.hpp
)
target_include_directories(zappy_gui_graphics_scene PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/raylib
        ${CMAKE_SOURCE_DIR}/gui/src
)
target_link_libraries(zappy_gui_graphics_scene PUBLIC
        zappy_shared
        zappy_gui_raylib
)
