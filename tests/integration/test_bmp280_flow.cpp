#include <gtest/gtest.h>

#include "BMP280_Wrapper.h"
#include "../mocks/stm32_hal_mock.h"

TEST(BMP280_Flow, InitializationSequence) {
    // Mock the BMP280 Chip ID register (0xD0). A real BMP280 should return 0x58.
    // This makes BMP280_Init() pass the "who am I" check during initialization.
    mock_bmp280_regs[0xD0] = 0x58;

    // Create a mock I2C handle (dummy instance is fine for the HAL mock layer).
    I2C_HandleTypeDef mock_i2c = {0};

    // Create a C wrapper handle for the BMP280 driver using the mock I2C interface.
    BMP280_Handle wrapper = BMP280_Create(&mock_i2c);

    // Run initialization (reset -> chip ID check -> calibration read -> config write).
    int success = BMP280_Init(wrapper);

    // Expect initialization to succeed.
    EXPECT_EQ(success, 1);
}
