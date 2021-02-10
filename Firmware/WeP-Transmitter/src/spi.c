#include <avr/io.h>
#include <include/spi.h>


/* Riistvaraprojekt UT 2020/21 - Rando Hinn,saatjamooduli SPI libra */


void SPI_init() {
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRB &= ~(1 << DDB3);
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
	PORTB |= (1 << PINB0);
}

void SPI_unset_cs() {
    PORTB &= ~(1 << PINB0);
}

void SPI_set_cs(){
    PORTB |= (1 << PINB0);
}

void SPI_send(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF) ));
}