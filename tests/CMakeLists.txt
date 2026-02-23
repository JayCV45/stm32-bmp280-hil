cmake_minimum_required(VERSION 3.10)
project(BMP280_Tests)

find_package(GTest REQUIRED)

set(BMP280_DRIVER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/Drivers/BMP280/inc)
set(BMP280_DRIVER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/Drivers/BMP280/src)
set(BMP280_TEST_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_library(bmp280_driver STATIC
    ${BMP280_DRIVER_SRC_DIR}/BMP280.cpp
    ${BMP280_DRIVER_SRC_DIR}/BMP280_Wrapper.cpp
)

target_compile_definitions(bmp280_driver PUBLIC UNIT_TEST)
target_include_directories(bmp280_driver
    PUBLIC
        ${BMP280_DRIVER_INC_DIR}
    PRIVATE
        ${BMP280_TEST_MOCK_DIR}
)

add_subdirectory(unit)
add_subdirectory(integration)
