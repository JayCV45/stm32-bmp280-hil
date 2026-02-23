add_executable(unit_tests test_bmp280_math.cpp ../../src/bmp280.c)
target_link_libraries(unit_tests GTest::GTest GTest::Main)
