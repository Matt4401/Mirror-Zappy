set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(zappy_server_objects OBJECT
        src/game/World.cpp
        src/game/World.hpp
        src/Core.cpp
        src/Core.hpp
        src/guiCommand/IGuiCommand.hpp
        src/guiCommand/Mct.cpp
        src/guiCommand/Mct.hpp
        src/guiCommand/Msz.cpp
        src/guiCommand/Msz.hpp
        src/guiCommand/Sgt.cpp
        src/guiCommand/Sgt.hpp
        src/guiCommand/Bct.cpp
        src/guiCommand/Bct.hpp
        src/guiCommand/Tna.cpp
        src/guiCommand/Tna.hpp
        src/command/ICommand.hpp
        src/command/ACommand.hpp
        src/command/CommandFactory.cpp
        src/command/CommandFactory.hpp
        src/command/Forward.cpp
        src/command/Forward.hpp
        src/game/Player.cpp
        src/game/Player.hpp
        src/command/Left.cpp
        src/command/Left.hpp
        src/command/Right.cpp
        src/command/Right.hpp
        src/game/Team.cpp
        src/game/Team.hpp
)
target_include_directories(zappy_server_objects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/shared/
        ${CMAKE_SOURCE_DIR}/network/
)
target_link_libraries(zappy_server_objects PUBLIC
        zappy_shared
        zappy_parser
        network
)

add_executable(zappy_server
        src/main.cpp
)
target_link_libraries(zappy_server PRIVATE
        zappy_server_objects
        zappy_parser
)
