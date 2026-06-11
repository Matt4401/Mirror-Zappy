include(GoogleTest)
find_package(Python3 REQUIRED)

set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(unit_tests
        src/gui/Parser.cpp
        src/network/Socket.cpp
        src/network/SessionManager.cpp
        src/server/Core.cpp
        src/server/command/CommandFactory.cpp
        src/server/command/Forward.cpp
        src/server/Player.cpp
        src/server/command/Right.cpp
        src/server/command/Left.cpp
)
target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(unit_tests PRIVATE
        zappy_shared
        zappy_server_objects
        zappy_gui_core
        # zappy_ai_objects
        GTest::gtest
        GTest::gmock
        GTest::gmock_main
)
target_compile_definitions(unit_tests
        PRIVATE
        FIXTURES_PATH="${FIXTURES_DIR}"
)

add_test(NAME python_tests
    COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/src/ai/test_main.py
)

set_tests_properties(python_tests PROPERTIES 
    ENVIRONMENT "PYTHONPATH=${CMAKE_SOURCE_DIR}/ai/src"
)

gtest_discover_tests(unit_tests)
