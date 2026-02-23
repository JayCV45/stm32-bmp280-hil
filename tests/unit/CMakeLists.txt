add_executable(unit_tests test_bmp280_math.cpp)
target_link_libraries(unit_tests PRIVATE bmp280_driver GTest::gtest_main)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/mocks)
