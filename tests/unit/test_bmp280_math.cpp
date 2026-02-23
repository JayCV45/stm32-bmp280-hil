#include <gtest/gtest.h>
#include "BMP280.hpp"

#include "../mocks/stm32_hal_mock.h"

TEST(BMP280_Logic, TemperatureFormulaCheck) {
    I2C_HandleTypeDef mock_i2c = {0};
    BMP280 sensor(&mock_i2c);

    // 1) BMP280 chip ID (most drivers require this)
    mock_bmp280_regs[0xD0] = 0x58;

    // Optional but often helps if your init polls status
    mock_bmp280_regs[0xF3] = 0x00; // status: not busy

    // 2) Provide calibration bytes your init reads.
    // If init reads the whole 0x88..0xA1 block, make sure those exist.
    // At least fill the block with 0 so reads don't fail / return garbage.
    for (int a = 0x88; a <= 0xA1; ++a) mock_bmp280_regs[a] = 0x00;

    // Your specific calibration values (little-endian)
    mock_bmp280_regs[0x88] = 0x70;
    mock_bmp280_regs[0x89] = 0x6B;
    mock_bmp280_regs[0x8A] = 0x43;
    mock_bmp280_regs[0x8B] = 0x67;
    mock_bmp280_regs[0x8C] = 0x18;
    mock_bmp280_regs[0x8D] = 0xFC;

    ASSERT_TRUE(sensor.init());

    mock_bmp280_regs[0xFA] = 0x7E;
    mock_bmp280_regs[0xFB] = 0xED;
    mock_bmp280_regs[0xFC] = 0x00;

    float result = sensor.readTemperature();
    EXPECT_NEAR(result, 25.08f, 0.01f);
}
