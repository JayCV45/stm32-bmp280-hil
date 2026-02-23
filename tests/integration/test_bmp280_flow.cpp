#include <gtest/gtest.h>

#include "BMP280_Wrapper.h"
#include "../mocks/stm32_hal_mock.h"

TEST(BMP280_Flow, InitializationSequence) {
    mock_bmp280_regs[0xD0] = 0x58;

    I2C_HandleTypeDef mock_i2c = {0};
    BMP280_Handle wrapper = BMP280_Create(&mock_i2c);

    int success = BMP280_Init(wrapper);

    EXPECT_EQ(success, 1);
}
