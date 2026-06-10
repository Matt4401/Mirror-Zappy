include(FetchContent)
include(InternetCheck)

check_internet_connection()

macro(setup_googletest)
    find_package(GTest QUIET)

    if (TARGET GTest::gtest AND TARGET GTest::gmock AND TARGET GTest::gmock_main)
        message(STATUS "GoogleTest found on system. Unit tests will be built.")
    elseif (INTERNET_AVAILABLE)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG v1.17.0
        )

        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(googletest)

        if (TARGET gtest AND NOT TARGET GTest::gtest)
            add_library(GTest::gtest ALIAS gtest)
        endif()
        if (TARGET gmock AND NOT TARGET GTest::gmock)
            add_library(GTest::gmock ALIAS gmock)
        endif()
        if (TARGET gmock_main AND NOT TARGET GTest::gmock_main)
            add_library(GTest::gmock_main ALIAS gmock_main)
        endif()
    else()
        message(STATUS "GoogleTest not available. Unit tests will not be built.")
    endif()
endmacro()
