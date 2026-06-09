add_library(zappy_gui_cli
        Parser.cpp
)
target_sources(zappy_gui_cli PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES Parser.hpp
)
target_include_directories(zappy_gui_cli PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(zappy_gui_cli PUBLIC
        zappy_shared
        zappy_gui_raylib
)
