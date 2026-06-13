add_library(zappy_gui_events INTERFACE)

target_sources(zappy_gui_events INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES EventDispatcher.hpp
)

target_include_directories(zappy_gui_events INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(zappy_gui_events INTERFACE
        zappy_shared
)
