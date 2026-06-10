add_library(zappy_gui_graphics
        Render.cpp
        Tile3D.cpp
)
target_sources(zappy_gui_graphics PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Render.hpp
        Tile3D.hpp
)
target_include_directories(zappy_gui_graphics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/raylib
)
target_link_libraries(zappy_gui_graphics PUBLIC
        zappy_shared
        zappy_gui_raylib
)
