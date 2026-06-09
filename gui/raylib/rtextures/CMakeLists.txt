add_library(zappy_gui_raylib_rtextures INTERFACE)
target_sources(zappy_gui_raylib_rtextures INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Image.hpp
                Texture2D.hpp
)
target_include_directories(zappy_gui_raylib_rtextures INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rtextures INTERFACE
        zappy_gui_raylib_external
)
