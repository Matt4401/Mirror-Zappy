add_library(zappy_gui_raylib_rmodels
        Material.cpp
)
target_sources(zappy_gui_raylib_rmodels PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES Material.hpp
        Mesh.hpp
        Model.hpp
        ModelAnimation.hpp
        Shader.hpp
)
target_include_directories(zappy_gui_raylib_rmodels PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(zappy_gui_raylib_rmodels PUBLIC
        zappy_gui_raylib_external
        zappy_gui_raylib_rcore
        zappy_gui_raylib_rtextures
)
