add_library(network SHARED
    src/socket/BaseSocket.cpp
    src/socket/ClientSocket.cpp
    src/socket/ServerSocket.cpp
    src/SessionManager.cpp
    src/exception/ConnectError.hpp
    src/exception/PollError.hpp
    src/exception/SocketError.hpp
)

target_include_directories(network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/shared/
)
target_link_libraries(network PUBLIC
        zappy_shared
)
