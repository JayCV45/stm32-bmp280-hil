#ifndef STM32_HAL_MOCK_H
#define STM32_HAL_MOCK_H

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Minimal STM32 HAL type stubs used by your driver ----
typedef struct {
  int _unused;
} I2C_HandleTypeDef;

typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

// ---- Shared mock register map (ONE instance across all translation units) ----
extern uint8_t mock_bmp280_regs[256];

// Optional helper to reset between tests
void mock_bmp280_reset_regs(void);

// ---- HAL I2C memory APIs used by the driver ----
HAL_StatusTypeDef HAL_I2C_Mem_Read(
    I2C_HandleTypeDef* hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t* pData,
    uint16_t Size,
    uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef* hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    const uint8_t* pData,
    uint16_t Size,
    uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif  
