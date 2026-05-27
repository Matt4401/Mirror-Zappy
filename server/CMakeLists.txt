set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# add_library(zappy_server_objects OBJECT
# )
# target_include_directories(zappy_server_objects PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#         ${CMAKE_SOURCE_DIR}/shared/include
# )
# target_link_libraries(zappy_server_objects PUBLIC
#         zappy_shared
# )

add_executable(zappy_server
        src/main.cpp
)
# target_link_libraries(zappy_server PRIVATE
#         zappy_server_objects
# )
