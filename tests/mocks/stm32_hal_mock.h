#ifndef STM32_HAL_MOCK_H
#define STM32_HAL_MOCK_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

typedef struct {
    uint32_t Instance;
} I2C_HandleTypeDef;

static uint8_t mock_bmp280_regs[256];

static inline HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
                                                  uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddSize;
    (void)Timeout;

    if (pData == 0) {
        return HAL_ERROR;
    }

    memcpy(pData, &mock_bmp280_regs[MemAddress], Size);
    return HAL_OK;
}

static inline HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
                                                   uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddSize;
    (void)Timeout;

    if (pData == 0) {
        return HAL_ERROR;
    }

    memcpy(&mock_bmp280_regs[MemAddress], pData, Size);
    return HAL_OK;
}

static inline void HAL_Delay(uint32_t Delay) {
    (void)Delay;
}

#ifdef __cplusplus
}
#endif

#endif
