add_library(zappy_gui_raylib_rmath INTERFACE)
target_sources(zappy_gui_raylib_rmath INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Vector2.hpp
                Vector3.hpp
)
target_include_directories(zappy_gui_raylib_rmath INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rmath INTERFACE
        zappy_gui_raylib_external
)
