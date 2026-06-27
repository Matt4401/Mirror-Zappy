add_subdirectory(components)

add_library(zappy_gui_game
        ItemBag.cpp
        Team.cpp
        GameModel.cpp
        Player.cpp
        NameGenerator.cpp
)
target_sources(zappy_gui_game PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        Player.hpp
        Team.hpp
        ItemBag.hpp
        GameModel.hpp
        Egg.hpp
        NameGenerator.hpp
)
target_include_directories(zappy_gui_game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/raylib
        ${CMAKE_SOURCE_DIR}/gui/src
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(zappy_gui_game PUBLIC
        zappy_shared
        zappy_gui_raylib
        zappy_gui_game_components
        zappy_server_objects
)
target_link_libraries(zappy_gui_game PRIVATE
        zappy_gui_assets
)
