cmake_minimum_required(VERSION 3.10)
project(BMP280_Tests)

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../inc ./mocks)

add_subdirectory(unit)
add_subdirectory(integration)
