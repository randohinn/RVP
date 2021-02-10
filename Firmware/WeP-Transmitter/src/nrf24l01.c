#include <include/nrf24l01.h>
#include <include/spi.h>
#define F_CPU 16000000UL
#include <util/delay.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn,saatjamooduli nrf24l01 raadioside libra */


void nrf24l01_write_register(uint8_t reg, uint8_t* data, uint8_t bytecount) {
	reg =  NRF_WRITE + reg;
	SPI_unset_cs();
	SPI_send(reg);
	for(int i = 0; i < bytecount; i++) {
		SPI_send(data[i]);
	}
	SPI_set_cs();
}

void nrf24l01_init(uint8_t address_width, uint8_t* address, uint8_t channel, uint8_t payload_width, uint8_t mode) {
	
	// Setup CE pin that enables RF activity (not to be confused with the CSN pin, which is for the SPI)
	DDRB |= (1 << DDB7);
	PORTB &= ~(1 << PORTB7);

	uint8_t config_values[1];
	
	//Enable Auto-ACK
	config_values[0] = 0x01;
	nrf24l01_write_register(EN_AA, config_values, 1);
	
	//Setup automatic retries 750uS delay, up to 15 times
	config_values[0] = 0x2F;
	nrf24l01_write_register(SETUP_RETR, config_values, 1);
	
	//Enable data pipe 0 for recieve communications
	config_values[0] = 0x01;
	nrf24l01_write_register(EN_RXADDR, config_values, 1);
	
	//Set address length as 5 bytes
	config_values[0] = address_width;
	nrf24l01_write_register(SETUP_AW, config_values,1);
	
	//Setup a radio channel
	config_values[0] = channel;
	nrf24l01_write_register(RF_CH, config_values,1);

	//Setup in -0dB long range mode
	config_values[0] = 0x07;
	nrf24l01_write_register(RF_SETUP, config_values,1);
	
	//Setup addresses for recieve pipe 0 and tx
	nrf24l01_write_register(RX_ADDR_P0,address,address_width);
	nrf24l01_write_register(TX_ADDR,address,address_width);
	
	//Setup payload width (bytes/package)
	config_values[0] = payload_width;
	nrf24l01_write_register(RX_PW_P0,config_values,1);
	
	//Bootup and config
	config_values[0] = (1 << PWR_UP) | (1 << CRCO) | (1 << EN_CRC) | (1 << MASK_MAX_RT);
	
	if(mode == 0) {
		config_values[0] = 0x1E;
		//config_values[0] |=  (1 << MASK_TX_DS) | (1 << MASK_RX_DR);
	} else {
		config_values[0] |= (1 << PRIM_RX) | (1 << MASK_TX_DS);
	}
	nrf24l01_write_register(CONFIG, config_values, 1);
} 

void nrf24l01_transmit(uint8_t* payload, uint8_t payload_size) {
	SPI_unset_cs();
	SPI_send(FLUSH_TX);
	SPI_set_cs();
	
	SPI_unset_cs();
	SPI_send(W_TX_PAYLOAD);
	for(int i = 0; i < payload_size; i++) {
		SPI_send(payload[i]);
	}
	SPI_set_cs();
	_delay_ms(10);
	PORTB |= (1 << PORTB7);
	_delay_ms(20);
	PORTB &= ~(1 << PORTB7);
	_delay_ms(10);
}

void nrf24l01_read_register(uint8_t reg, uint8_t* data, uint8_t bytecount) {
	SPI_unset_cs();
	SPI_send(reg);
	for(int i = 0; i < bytecount; i++) {
		SPI_send(NOP);
		data[i] = SPDR;
	}
	SPI_set_cs();
}

void nrf24l01_reset_interrupts() {
	uint8_t val[1];
	val[0] = (1 << MAX_RT) | (1 << TX_DS) | (1 << RX_DR);
	nrf24l01_write_register(STATUS,val,1);
}
