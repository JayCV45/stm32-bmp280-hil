#ifndef BMP280_WRAPPER_H
#define BMP280_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* BMP280_Handle;

BMP280_Handle BMP280_Create(void* hi2c_handle);
int BMP280_Init(BMP280_Handle handle);
float BMP280_ReadTemperature(BMP280_Handle handle);

#ifdef __cplusplus
}
#endif
#endif
