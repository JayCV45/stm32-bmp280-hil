# Build the unit test binary from the math-focused test source file.
add_executable(unit_tests test_bmp280_math.cpp)

# Link against:
#  - bmp280_driver: your driver/library under test
#  - GTest::gtest_main: GoogleTest main() entrypoint + gtest runtime
target_link_libraries(unit_tests PRIVATE bmp280_driver GTest::gtest_main)

# Add include path for test-only headers (e.g., mocked STM32 HAL headers).
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/mocks)
