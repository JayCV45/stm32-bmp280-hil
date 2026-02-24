# Build the integration test binary from the test source file.
add_executable(integration_tests test_bmp280_flow.cpp)

# Link against:
#  - bmp280_driver: your driver/library under test
#  - GTest::gtest_main: GoogleTest main() entrypoint + gtest runtime
target_link_libraries(integration_tests PRIVATE bmp280_driver GTest::gtest_main)

# Add include path for test-only headers (e.g., mocked STM32 HAL headers).
target_include_directories(integration_tests PRIVATE ${CMAKE_SOURCE_DIR}/mocks)
