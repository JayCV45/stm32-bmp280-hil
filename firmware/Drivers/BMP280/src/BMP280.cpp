#include "BMP280.hpp"

BMP280::BMP280(I2C_HandleTypeDef *hi2c, uint16_t devAddr) 
    : _hi2c(hi2c), _devAddr(devAddr), t_fine(0) {
    // Initialize temperature calibration fields to zero (safe defaults)
    calib.dig_T1 = 0;
    calib.dig_T2 = 0;
    calib.dig_T3 = 0;
}

bool BMP280::init() {
    // 1) Reset the chip to return registers to default state
    // Writing 0xB6 to register 0xE0 triggers a soft reset.
    uint8_t reset_cmd = 0xB6;
    if (HAL_I2C_Mem_Write(_hi2c, _devAddr, 0xE0, 1, &reset_cmd, 1, 100) != HAL_OK) {
        return false;
    }
    HAL_Delay(10); // Wait for the reset sequence to complete

    // 2) Verify Chip ID (register 0xD0 should read 0x58 for BMP280)
    uint8_t chipID = 0;
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0xD0, 1, &chipID, 1, 100) != HAL_OK) {
        return false;
    }
    if (chipID != 0x58) {
        return false; 
    }

    // 3) Read factory calibration values (required for compensation formulas)
    readCalibration();

    // 4) Configure ctrl_meas (register 0xF4)
    // 0x27 corresponds to:
    //   osrs_t = 001 (x1 oversampling)
    //   osrs_p = 001 (x1 oversampling)
    //   mode   = 11  (Normal mode)
    uint8_t config = 0x27; 
    if (HAL_I2C_Mem_Write(_hi2c, _devAddr, 0xF4, 1, &config, 1, 100) != HAL_OK) {
        return false;
    }
    
    return true;
}

void BMP280::readCalibration() {
    // BMP280 temperature calibration parameters are stored starting at 0x88.
    // For temperature-only, we need dig_T1..dig_T3 (6 bytes total).
    uint8_t data[6];
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0x88, 1, data, 6, 100) == HAL_OK) {
        calib.dig_T1 = (uint16_t)((data[1] << 8) | data[0]);
        calib.dig_T2 = (int16_t)((data[3] << 8) | data[2]);
        calib.dig_T3 = (int16_t)((data[5] << 8) | data[4]);
    }
}

float BMP280::readTemperature() {
    // Temperature raw data is 20-bit across registers 0xFA..0xFC (MSB, LSB, XLSB).
    uint8_t data[3];
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0xFA, 1, data, 3, 100) != HAL_OK) {
        return -99.99f;      // Sentinel value: if you see this on UART, I2C read likely failed.
    }

    // Assemble 20-bit raw temperature value
    int32_t adc_T = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));

    // Bosch datasheet compensation algorithm (temperature)
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
    
    // t_fine is kept for pressure calculation as well (even if not used yet)
    t_fine = var1 + var2;
    
    // Convert to temperature in °C (datasheet outputs value scaled by 100)
    float T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f;
}
