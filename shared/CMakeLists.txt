set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(zappy_shared STATIC
    src/network/ClientSocket.cpp
    include/network/ClientSocket.hpp
    src/network/BaseSocket.cpp
    include/network/BaseSocket.hpp
    src/exception/Exception.cpp
    include/exception/Exception.hpp
    include/exception/ParserError.hpp
    include/exception/SocketError.hpp
)

target_include_directories(zappy_shared PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
