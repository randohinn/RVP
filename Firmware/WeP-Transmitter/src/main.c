#include <avr/io.h>
#include <include/spi.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <include/nrf24l01.h>
#include <avr/interrupt.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn,saatjamooduli peakood*/


volatile uint8_t data[10];
volatile uint8_t dataidx = 0;
volatile uint8_t data_good = 0;

void init_serial() {
	DDRD |= (1 << PIND3);
	DDRD &= ~(1 << PIND2);

    UCSR1A = 0;   
	UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);   // Turn on the transmission and reception circuitry
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
    UBRR1H = 0;
    UBRR1L = 25;
}

void serial_putc(char c) {
	while ((UCSR1A & (1<<UDRE1))==0);
	UDR1 = c;
}

void serial_send(char* send) {
    while (*send) {
        serial_putc(*send++);
    }
}

ISR(USART1_RX_vect)
{
    char c = UDR1;
    if(c != '\r') {
        data[dataidx] = c;
        dataidx++;
        if(dataidx == 10) {
            dataidx = 0;
        }
    } else {
        PORTF |= (1 << PORTF0);
        data_good = 1;
    }
}


int main() {
	
	init_serial();
    sei();

	//DEBUG led
	DDRF = (1 << DDF0);
	
	SPI_init();
	_delay_ms(1000);
	
	uint8_t address[5];
	for(int i = 0; i < 5; i++) {
		address[i] = 0x12+i;
    }
	
	nrf24l01_init(5, address, 110, 10 ,MODE_TRANSMIT);
	
	nrf24l01_transmit(data,10);
	nrf24l01_reset_interrupts();
		

    while(1) {
        if(data_good) {
        	PORTF |= (1 << PORTF0);
            nrf24l01_transmit(data,10);
            nrf24l01_reset_interrupts();
            PORTF &= ~(1 << PORTF0);
            dataidx = 0;
            data_good = 0;
            serial_putc('K');
        }
	}
    return 0;
}
