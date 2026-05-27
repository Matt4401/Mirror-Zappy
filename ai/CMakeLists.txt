set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# add_library(zappy_ai_objects OBJECT
# )
# target_include_directories(zappy_ai_objects PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#         ${CMAKE_SOURCE_DIR}/shared/include
# )
# target_link_libraries(zappy_ai_objects PUBLIC
#         zappy_shared
# )

add_executable(zappy_ai
        src/main.cpp
)
# target_link_libraries(zappy_ai PRIVATE
#         zappy_ai_objects
# )
