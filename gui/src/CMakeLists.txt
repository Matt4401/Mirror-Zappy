add_library(zappy_gui_assets
        graphics/AssetManager.cpp
)
target_sources(zappy_gui_assets PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES graphics/AssetManager.hpp
)
target_include_directories(zappy_gui_assets PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/raylib
)
target_link_libraries(zappy_gui_assets PUBLIC
        zappy_gui_raylib
)

add_subdirectory(game)
add_subdirectory(events)
add_subdirectory(graphics)
add_subdirectory(network)
add_subdirectory(ui)

add_library(zappy_gui_core
        Core.cpp
)
target_sources(zappy_gui_core PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Core.hpp
)
target_include_directories(zappy_gui_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(zappy_gui_core PUBLIC
        zappy_shared
        zappy_gui_raylib
        zappy_gui_graphics
        network
        zappy_gui_network
        zappy_gui_events
        zappy_gui_ui
)
