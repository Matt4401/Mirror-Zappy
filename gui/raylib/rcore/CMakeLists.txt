add_library(zappy_gui_raylib_rcore INTERFACE)
target_sources(zappy_gui_raylib_rcore INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Window.hpp
                Camera.hpp
                Event.hpp
                Matrix.hpp
)
target_include_directories(zappy_gui_raylib_rcore INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rcore INTERFACE
        zappy_gui_raylib_external
)
