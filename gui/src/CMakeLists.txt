add_subdirectory(cli)
add_subdirectory(graphics)
add_subdirectory(network)

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
        zappy_gui_cli
        zappy_gui_graphics
        zappy_gui_network
)
