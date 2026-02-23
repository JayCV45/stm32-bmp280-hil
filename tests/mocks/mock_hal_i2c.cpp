#pragma once
#include <stdint.h>
#include <string.h>

// 1. Basic HAL Types
typedef enum { 
    HAL_OK = 0x00U, 
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U 
} HAL_StatusTypeDef;

typedef struct { 
    uint32_t Instance; // Some drivers check this pointer
} I2C_HandleTypeDef;

// 2. Global "Virtual Sensor" memory
// We use 'inline' or 'extern' to avoid multiple definition errors
inline uint8_t mock_bmp280_regs[256];

// 3. Mocked HAL Functions
// Implementation of Read: Copy from our virtual registers to the driver buffer
inline HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, 
                                          uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    if (pData == nullptr) return HAL_ERROR;
    memcpy(pData, &mock_bmp280_regs[MemAddress], Size);
    return HAL_OK;
}

// Implementation of Write: Copy from the driver buffer to our virtual registers
inline HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, 
                                           uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    if (pData == nullptr) return HAL_ERROR;
    memcpy(&mock_bmp280_regs[MemAddress], pData, Size);
    return HAL_OK;
}
