add_library(zappy_gui_raylib_rtextures
        Texture2D.cpp
)
target_sources(zappy_gui_raylib_rtextures PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Image.hpp
                Texture2D.hpp
)
target_include_directories(zappy_gui_raylib_rtextures PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rtextures PUBLIC
        zappy_gui_raylib_external
)
