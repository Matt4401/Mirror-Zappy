add_library(zappy_gui_context INTERFACE
)
target_sources(zappy_gui_context INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES EventContext.hpp
)
target_include_directories(zappy_gui_context INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(zappy_gui_context INTERFACE
        zappy_gui_raylib_rcore
)
