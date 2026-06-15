add_library(zappy_gui_game_components INTERFACE)
target_sources(zappy_gui_game_components INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES components/AObject.hpp
        components/Deraumere.hpp
        components/Food.hpp
        components/IObject.hpp
        components/Linemate.hpp
        components/Mendiane.hpp
        components/Phiras.hpp
        components/Sibur.hpp
        components/Thystame.hpp
)
target_include_directories(zappy_gui_game_components INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/gui/raylib
)
target_link_libraries(zappy_gui_game_components INTERFACE
        zappy_gui_raylib
)
