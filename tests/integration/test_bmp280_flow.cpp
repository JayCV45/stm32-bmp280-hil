#include <gtest/gtest.h>
#include "BMP280_Wrapper.h"
#include "../mocks/stm32_hal_mock.h"

TEST(BMP280_Flow, InitializationSequence) {
    // 1. Arrange: Set the "Virtual Sensor" ID register to the correct value
    mock_bmp280_regs[0xD0] = 0x58; 

    BMP280_Wrapper wrapper;

    // 2. Act: Try to initialize
    bool success = wrapper.begin();

    // 3. Assert: Did the driver correctly read 0x58 and return true?
    EXPECT_TRUE(success);
}
