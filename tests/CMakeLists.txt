cmake_minimum_required(VERSION 3.15)

include(FetchContent)
FetchContent_Declare(
  googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(TokenizerTest 
    TokenizerTest.cpp
)

target_link_libraries(TokenizerTest PRIVATE 
    TokenizerLib
    GTest::gtest
    GTest::gtest_main
)

# 4. Register it with CMake's test runner engine
include(GoogleTest)
gtest_discover_tests(TokenizerTest)
