#include "BMP280_Wrapper.h"
#include "BMP280.hpp"

extern "C" {
    BMP280_Handle BMP280_Create(void* hi2c_handle) {
        return (BMP280_Handle) new BMP280((I2C_HandleTypeDef*)hi2c_handle);
    }

    int BMP280_Init(BMP280_Handle handle) {
        return ((BMP280*)handle)->init() ? 1 : 0;
    }

    float BMP280_ReadTemperature(BMP280_Handle handle) {
        return ((BMP280*)handle)->readTemperature();
    }
}
