#include <avr/io.h>
#include <include/spi.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli SPI libra */


void SPI_init() {
	//EINK CS, SCK & MOSI out
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	// RF CS
	DDRC |= (1 << DDC6);
	
	DDRB &= ~(1 << DDB3);
	
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	//RF CS High
    PORTC |= (1 << PC6);
	
	//EINK CS HIGH
	PORTB |= (1 << PB0);
}

void SPI_unset_RF_cs() {
    PORTC &= ~(1 << PINC6);
}

void SPI_set_RF_cs(){
    PORTC |= (1 << PINC6);
}

void SPI_unset_EINK_cs() {
	PORTB &= ~(1 << PB0);
}

void SPI_set_EINK_cs() {
	PORTB |= (1 << PB0);
}

void SPI_send(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF) ));
}