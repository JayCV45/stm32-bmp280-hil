add_executable(integration_tests test_bmp280_flow.cpp)
target_link_libraries(integration_tests PRIVATE bmp280_driver GTest::gtest_main)
target_include_directories(integration_tests PRIVATE ${CMAKE_SOURCE_DIR}/mocks)
