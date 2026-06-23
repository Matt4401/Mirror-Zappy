add_library(zappy_gui_graphics_scene
        Map.cpp
        Skybox3D.cpp
        Tile3D.cpp
        WorldManager.cpp
)
target_sources(zappy_gui_graphics_scene PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Map.hpp
        Skybox3D.hpp
        Tile3D.hpp
        WorldManager.hpp
)
target_include_directories(zappy_gui_graphics_scene PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/gui/raylib
        ${CMAKE_SOURCE_DIR}/gui/src
)
target_link_libraries(zappy_gui_graphics_scene PUBLIC
        zappy_shared
        zappy_gui_raylib
        zappy_gui_game
        zappy_gui_game_components
        zappy_gui_events
)
