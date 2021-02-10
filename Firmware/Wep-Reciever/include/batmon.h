#ifndef BATMON_H
#define BATMON_H
#include <stdint.h>

#define BQ27441_UNSEAL_KEY	0x8000
#define BQ72441_I2C_ADDRESS 0x55
#define BQ27441_CONTROL_DEVICE_TYPE		0x01

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli peakood */


uint8_t batmon_init();
uint8_t batmon_config(uint16_t dcap);
uint8_t batmon_soc();

#endif

