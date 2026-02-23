#include "stm32_hal_mock.h"
#include <string.h>  // memset

// Single, shared mock register map definition
uint8_t mock_bmp280_regs[256] = {0};

void mock_bmp280_reset_regs(void) {
  memset(mock_bmp280_regs, 0, sizeof(mock_bmp280_regs));
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(
    I2C_HandleTypeDef* /*hi2c*/,
    uint16_t /*DevAddress*/,
    uint16_t MemAddress,
    uint16_t /*MemAddSize*/,
    uint8_t* pData,
    uint16_t Size,
    uint32_t /*Timeout*/) {

  if (pData == nullptr) return HAL_ERROR;
  if ((MemAddress + Size) > sizeof(mock_bmp280_regs)) return HAL_ERROR;

  memcpy(pData, &mock_bmp280_regs[MemAddress], Size);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef* /*hi2c*/,
    uint16_t /*DevAddress*/,
    uint16_t MemAddress,
    uint16_t /*MemAddSize*/,
    const uint8_t* pData,
    uint16_t Size,
    uint32_t /*Timeout*/) {

  if (pData == nullptr) return HAL_ERROR;
  if ((MemAddress + Size) > sizeof(mock_bmp280_regs)) return HAL_ERROR;

  memcpy(&mock_bmp280_regs[MemAddress], pData, Size);
  return HAL_OK;
}
