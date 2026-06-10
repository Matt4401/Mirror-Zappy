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
    include/exception/ParsingError.hpp
        include/parsing/Parser.hpp
        include/parsing/IParseStrategy.hpp
        include/parsing/strategy/GUIStrategy.hpp
        include/parsing/strategy/ServerStrategy.hpp
        src/parsing/strategy/GUIStrategy.cpp
        src/parsing/strategy/ServerStrategy.cpp
        include/encapsulation/GetoptWrapper.hpp
        src/encapsulation/GetoptWrapper.cpp
)

target_include_directories(zappy_shared PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
