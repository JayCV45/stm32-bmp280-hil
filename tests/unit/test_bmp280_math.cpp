#include <gtest/gtest.h>
#include "BMP280.hpp" // Included via the "Gate"

TEST(BMP280_Logic, TemperatureFormulaCheck) {
    // 1. Arrange: Create a driver instance
    BMP280 sensor;
    
    // 2. Act: Call your math function with a known raw value 
    // (Example: datasheet says raw 519888 should be 25.08°C)
    float result = sensor.compensate_T(519888); 

    // 3. Assert: Verify the logic
    EXPECT_NEAR(result, 25.08, 0.01);
}
