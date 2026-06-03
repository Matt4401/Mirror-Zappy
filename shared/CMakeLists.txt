set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(zappy_shared STATIC
    src/Socket.cpp
    src/Socket.hpp
    src/exception/Exception.cpp
    src/exception/Exception.hpp
    src/exception/SocketError.hpp
)

target_include_directories(zappy_shared PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
