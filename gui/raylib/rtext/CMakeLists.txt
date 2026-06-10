add_library(zappy_gui_raylib_rtext
        Font.cpp
)
target_sources(zappy_gui_raylib_rtext PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Font.hpp
                Text.hpp
)
target_include_directories(zappy_gui_raylib_rtext PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rtext PUBLIC
        zappy_gui_raylib_external
)
