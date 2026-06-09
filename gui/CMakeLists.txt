set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(raylib)
add_subdirectory(src)

# add_library(zappy_gui_objects OBJECT
# )
# target_include_directories(zappy_gui_objects PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#         ${CMAKE_SOURCE_DIR}/shared/include
# )
# target_link_libraries(zappy_gui_objects PUBLIC
#         zappy_shared
# )

add_executable(zappy_gui
        src/main.cpp
)
target_link_libraries(zappy_gui PRIVATE
        zappy_gui_core
        zappy_gui_raylib
)
