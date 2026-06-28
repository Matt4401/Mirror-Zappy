include(GoogleTest)
find_package(Python3 REQUIRED)

set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(unit_tests
        src/server/parsing/Parser.cpp
        src/gui/parsing/Parser.cpp
        src/gui/events/EventDispatcherTest.cpp
        src/gui/world/WorldManagerTest.cpp
        src/gui/network/ClientTest.cpp
        src/network/Socket.cpp
        src/network/SessionManager.cpp
        src/server/Core.cpp
        src/server/command/CommandFactory.cpp
        src/server/command/Forward.cpp
        src/server/Player.cpp
        src/server/command/Right.cpp
        src/server/command/Left.cpp
        src/server/guiCommand/guiCommands.cpp
        src/server/guiCommand/guiCommandFactory.cpp
        src/server/World.cpp
        src/protocol/ProtocolTest.cpp
        src/server/command/Broadcast.cpp
        src/server/command/Eject.cpp
        src/server/command/Fork.cpp
        src/server/command/Take.cpp
        src/server/command/Set.cpp
        src/server/command/Inventory.cpp
        src/server/command/Look.cpp
        src/server/command/Incantation.cpp
        src/server/command/ConnectNbr.cpp
)
target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(unit_tests PRIVATE
        zappy_shared
        zappy_parser
        zappy_server_objects
        zappy_gui_audio
        zappy_gui_events
        zappy_gui_network
        zappy_gui_graphics_scene
        GTest::gtest
        GTest::gmock
        GTest::gmock_main
)
target_compile_definitions(unit_tests
        PRIVATE
        FIXTURES_PATH="${FIXTURES_DIR}"
)
#
add_test(NAME python_tests
        COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/src/ai/test_main.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_Connection.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_inventory_class.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_VisionManager.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_Logger.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_TickManager.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_broadcast_manager.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_parse_cmd.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_trantorian.py ${CMAKE_SOURCE_DIR}/tests/src/ai/test_send_cmd.py
)

set_tests_properties(python_tests PROPERTIES
        ENVIRONMENT "PYTHONPATH=${CMAKE_SOURCE_DIR}/ai/src"
)

gtest_discover_tests(unit_tests)
