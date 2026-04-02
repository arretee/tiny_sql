cmake_minimum_required(VERSION 3.10.0)
project(TinySQL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GoogleTest for testing
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Main project build 
add_executable( TinySQL 
    src/main.cpp 
    src/table.h src/table.cpp 
    src/tokenizer.h src/tokenizer.cpp
    src/parser.h    src/parser.cpp
)


# Setup testing
enable_testing()
add_executable( UnitTest
    tests/test_tokenizer.cpp src/tokenizer.h src/tokenizer.cpp
    
)

target_link_libraries(UnitTest GTest::gtest GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(UnitTest)