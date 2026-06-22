cmake_minimum_required(VERSION 3.28)
project(zappy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CTest)

option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

if (ENABLE_COVERAGE)
    message(STATUS "Code coverage instrumentation enabled (-g -O0 --coverage)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif ()

add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/zappy_ai
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/ai/zappy_ai.py ${CMAKE_SOURCE_DIR}/zappy_ai
        COMMAND chmod +x ${CMAKE_SOURCE_DIR}/zappy_ai
        DEPENDS ${CMAKE_SOURCE_DIR}/ai/zappy_ai.py
)

add_custom_target(prepare_script ALL DEPENDS ${CMAKE_SOURCE_DIR}/zappy_ai)

add_custom_target(tests_functional
        COMMAND bash run_tests.sh -v
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/functional_tests
        DEPENDS zappy_server
        COMMENT "Lancement des tests fonctionnels Zappy..."
        USES_TERMINAL
)

add_subdirectory(shared)
add_subdirectory(parser)
add_subdirectory(server)
add_subdirectory(gui)
add_subdirectory(network)


include(SetupGoogleTest)
setup_googletest()
if (TARGET gtest_main)
    enable_testing()
    add_subdirectory(tests)
endif ()
