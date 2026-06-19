add_library(zappy_gui_events STATIC
        EventDispatcher.cpp
)

target_sources(zappy_gui_events PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES EventDispatcher.hpp
        GuiEvents.hpp
)

target_include_directories(zappy_gui_events PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(zappy_gui_events PUBLIC
        zappy_shared
)
