#ifndef STM32_HAL_MOCK_H
#define STM32_HAL_MOCK_H

#pragma once
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Minimal HAL status codes used by the driver under test.
typedef enum {
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

// Minimal I2C handle type expected by the driver.
// Only the fields accessed by your code need to exist here.
typedef struct {
    uint32_t Instance;
} I2C_HandleTypeDef;

// Virtual "register map" for the mocked BMP280.
// This is declared as extern so it is shared across translation units.
// Define it once in a .c/.cpp file, e.g.:
//   uint8_t mock_bmp280_regs[256] = {0};
extern uint8_t mock_bmp280_regs[256];

static inline HAL_StatusTypeDef HAL_I2C_Mem_Read(
    I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size,
    uint32_t Timeout
) {
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddSize;
    (void)Timeout;

    // Validate destination buffer
    if (pData == 0) {
        return HAL_ERROR;
    }

    // Copy from the virtual register map into the read buffer
    memcpy(pData, &mock_bmp280_regs[MemAddress], Size);
    return HAL_OK;
}

static inline HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size,
    uint32_t Timeout
) {
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddSize;
    (void)Timeout;

    // Validate source buffer
    if (pData == 0) {
        return HAL_ERROR;
    }

    // Copy the written bytes into the virtual register map
    memcpy(&mock_bmp280_regs[MemAddress], pData, Size);
    return HAL_OK;
}

static inline void HAL_Delay(uint32_t Delay) {
    // No-op for unit tests (keeps tests fast and deterministic)
    (void)Delay;
}

#ifdef __cplusplus
}
#endif

#endif
