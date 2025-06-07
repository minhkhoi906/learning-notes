cmake_minimum_required(VERSION 3.10)
project(UnitTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")

# Add GoogleTest
find_package(GTest REQUIRED)
file(GLOB TEST_SOURCES "./*.cpp")

# Add your test file
add_executable(run_all_tests ${TEST_SOURCES})

# Link GTest and pthread
target_link_libraries(run_all_tests GTest::gtest GTest::gtest_main pthread)
