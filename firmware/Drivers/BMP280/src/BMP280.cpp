#include "BMP280.hpp"

BMP280::BMP280(I2C_HandleTypeDef *hi2c, uint16_t devAddr) 
    : _hi2c(hi2c), _devAddr(devAddr), t_fine(0) {
    // Khởi tạo cấu trúc calib về 0
    calib.dig_T1 = 0;
    calib.dig_T2 = 0;
    calib.dig_T3 = 0;
}

bool BMP280::init() {
    // 1. Reset chip để đưa các thanh ghi về trạng thái mặc định
    uint8_t reset_cmd = 0xB6;
    if (HAL_I2C_Mem_Write(_hi2c, _devAddr, 0xE0, 1, &reset_cmd, 1, 100) != HAL_OK) {
        return false;
    }
    HAL_Delay(10); // Đợi chip khởi động lại sau reset

    // 2. Kiểm tra Chip ID (Thanh ghi 0xD0 phải trả về 0x58 cho BMP280)
    uint8_t chipID = 0;
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0xD0, 1, &chipID, 1, 100) != HAL_OK) {
        return false;
    }
    if (chipID != 0x58) {
        return false; 
    }

    // 3. Đọc dữ liệu hiệu chuẩn (Calibration)
    readCalibration();

    // 4. Cấu hình chế độ hoạt động (Thanh ghi 0xF4 - ctrl_meas)
    // 0x27 tương đương: 
    // osrs_t = 001 (x1), osrs_p = 001 (x1), mode = 11 (Normal mode)
    uint8_t config = 0x27; 
    if (HAL_I2C_Mem_Write(_hi2c, _devAddr, 0xF4, 1, &config, 1, 100) != HAL_OK) {
        return false;
    }
    
    return true;
}

void BMP280::readCalibration() {
    // BMP280 lưu các thông số hiệu chuẩn từ thanh ghi 0x88 đến 0xA1
    // Để an toàn, chúng ta đọc một mảng dữ liệu thô rồi mới ghép bit
    uint8_t data[6];
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0x88, 1, data, 6, 100) == HAL_OK) {
        calib.dig_T1 = (uint16_t)((data[1] << 8) | data[0]);
        calib.dig_T2 = (int16_t)((data[3] << 8) | data[2]);
        calib.dig_T3 = (int16_t)((data[5] << 8) | data[4]);
    }
}

float BMP280::readTemperature() {
    uint8_t data[3];
    // Đọc 3 byte dữ liệu nhiệt độ thô từ thanh ghi 0xFA (msb, lsb, xlsb)
    if (HAL_I2C_Mem_Read(_hi2c, _devAddr, 0xFA, 1, data, 3, 100) != HAL_OK) {
        return -99.99f;      // If it appers on UART, it means I2C has issue
    }

    // Chuyển đổi dữ liệu thô sang số nguyên 20-bit
    int32_t adc_T = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));

    // Thuật toán bù trừ nhiệt độ từ Datasheet của Bosch
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    float T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f;
}
