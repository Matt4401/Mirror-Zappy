set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(zappy_shared
        src/exception/Exception.cpp
        include/exception/Exception.hpp
        include/exception/ParserError.hpp
        include/exception/ParsingError.hpp
)

target_include_directories(zappy_shared PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}

)