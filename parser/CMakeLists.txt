set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(zappy_parser STATIC
        src/encapsulation/GetoptWrapper.cpp
        src/parsing/strategy/ServerStrategy.cpp
        src/parsing/strategy/GUIStrategy.cpp
        include/encapsulation/GetoptWrapper.hpp
        include/parsing/strategy/ServerStrategy.hpp
        include/parsing/strategy/GUIStrategy.hpp
        include/parsing/IParseStrategy.hpp
        include/parsing/Parser.hpp
)

target_link_libraries(zappy_parser PUBLIC
        zappy_shared
)

target_include_directories(zappy_parser PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(zappy_parser PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}shared/include/
)