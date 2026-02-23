#ifndef BMP280_HPP
#define BMP280_HPP

// --- THE CI GATE ---
#ifdef UNIT_TEST
    #include "stm32_hal_mock.h" // This will be provided by your tests/mocks folder
#else
    #include "stm32f4xx_hal.h"  // The real HAL for your STM32F4
#endif
// -------------------

#include <stdint.h>

class BMP280 {
public:
    BMP280(I2C_HandleTypeDef *hi2c, uint16_t devAddr = (0x76 << 1));
    bool init();
    float readTemperature();

private:
    I2C_HandleTypeDef *_hi2c;
    uint16_t _devAddr;
    int32_t t_fine;

    struct {
        uint16_t dig_T1;
        int16_t  dig_T2, dig_T3;
    } calib;

    void readCalibration();
};

#endif
