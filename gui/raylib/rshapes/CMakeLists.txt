add_library(zappy_gui_raylib_rshapes INTERFACE)
target_sources(zappy_gui_raylib_rshapes INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Shapes.hpp
)
target_include_directories(zappy_gui_raylib_rshapes INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rshapes INTERFACE
        zappy_gui_raylib_external
)
