set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(zappy_parser STATIC
        src/encapsulation/GetoptWrapper.cpp
        src/strategy/ServerStrategy.cpp
        src/strategy/GUIStrategy.cpp
        include/encapsulation/GetoptWrapper.hpp
        include/strategy/ServerStrategy.hpp
        include/strategy/GUIStrategy.hpp
        include/IParseStrategy.hpp
        include/Parser.hpp
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