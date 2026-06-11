add_library(zappy_gui_raylib_rcore
        Camera.cpp
        Event.cpp
        Window.cpp
)
target_sources(zappy_gui_raylib_rcore PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Window.hpp
                Camera.hpp
                Event.hpp
                Matrix.hpp
)
target_include_directories(zappy_gui_raylib_rcore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/gui/src
)
target_link_libraries(zappy_gui_raylib_rcore PUBLIC
        zappy_gui_raylib_external
)
