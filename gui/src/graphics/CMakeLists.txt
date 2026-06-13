add_subdirectory(scene)

add_library(zappy_gui_graphics
        Render.cpp
        AssetManager.cpp
)
target_sources(zappy_gui_graphics PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Render.hpp AssetManager.hpp
)
target_include_directories(zappy_gui_graphics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/raylib
        ${CMAKE_SOURCE_DIR}/gui/src
)
target_link_libraries(zappy_gui_graphics PUBLIC
        zappy_shared
        zappy_gui_raylib
        zappy_gui_context
        zappy_gui_graphics_scene
        zappy_gui_events
)
