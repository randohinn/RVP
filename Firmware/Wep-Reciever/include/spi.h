#ifndef SPI_H
#define SPI_H
#include <avr/io.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli SPI libra */


void SPI_init();

void SPI_unset_RF_cs();
void SPI_set_RF_cs();

void SPI_unset_EINK_cs();
void SPI_set_EINK_cs();

void SPI_send(uint8_t data);


#endif