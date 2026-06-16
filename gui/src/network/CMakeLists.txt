add_library(zappy_gui_network
        Client.cpp
)
target_sources(zappy_gui_network PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        Client.hpp
)
target_include_directories(zappy_gui_network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gui/src
)
target_link_libraries(zappy_gui_network PUBLIC
        zappy_shared
        network
        zappy_gui_raylib
        zappy_parser
        zappy_gui_events
)
