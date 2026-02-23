#include <gtest/gtest.h>
#include "BMP280.hpp"

#include "../mocks/stm32_hal_mock.h"

TEST(BMP280_Logic, TemperatureFormulaCheck) {
    I2C_HandleTypeDef mock_i2c = {0};
    BMP280 sensor(&mock_i2c);

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
