#ifndef I2C_H
#define I2C_H

#include <util/twi.h>
#include <stdint.h>
#define F_CPU 16000000UL

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli eink libra */

void _wait_transmission_complete();
void i2c_init(unsigned long clk_hz);
void i2c_stop();
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_nack();
uint8_t i2c_read_ack();
void i2c_read_multiple(uint8_t write_address, uint8_t read_address, uint8_t register_address, uint8_t n_bytes, uint8_t repstart, uint8_t * result);
uint8_t i2c_read_single(uint8_t write_address, uint8_t read_address, uint8_t register_address, uint8_t ack);
#endif