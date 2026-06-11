add_library(zappy_gui_raylib_raudio
        Wave.cpp
)
target_sources(zappy_gui_raylib_raudio PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES   Sound.hpp
                Wave.hpp
)
target_include_directories(zappy_gui_raylib_raudio PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_raudio PUBLIC
        zappy_gui_raylib_external
)
