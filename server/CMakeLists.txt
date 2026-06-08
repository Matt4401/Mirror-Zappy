set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(zappy_server_objects OBJECT
    src/network/SessionManager.cpp
    src/network/SessionManager.hpp
    src/network/ServerSocket.cpp
    src/network/ServerSocket.hpp
    src/game/World.cpp
    src/game/World.hpp
    src/Core.cpp
    src/Core.hpp
    src/util/DataStructures.hpp
    src/command/ICommand.hpp
    src/command/ACommand.hpp
    src/command/CommandFactory.cpp
    src/command/CommandFactory.hpp
    src/command/Forward.cpp
    src/command/Forward.hpp
)
target_include_directories(zappy_server_objects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/shared/
)
target_link_libraries(zappy_server_objects PUBLIC
        zappy_shared
)

add_executable(zappy_server
        src/main.cpp
)
target_link_libraries(zappy_server PRIVATE
        zappy_server_objects
)
