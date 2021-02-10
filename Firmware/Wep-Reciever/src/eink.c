#include <include/eink.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <include/spi.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli eink libra */
/* baseerub matemaatika osas waveshare'i enda libradel, https://github.com/waveshare/e-Paper*/


void EINK_toggle_reset() {
	PORTD &= ~(1 << PD5);
	_delay_ms(10);
	PORTD |= (1 << PD5);
}

void EINK_set_data() {
	PORTD |= (1 << PD4);
}

void EINK_set_command() {
	PORTD &= ~(1 << PD4);
}

void EINK_send_command(unsigned char command) {
	EINK_set_command();
	SPI_unset_EINK_cs();
	SPI_send(command);
	SPI_set_EINK_cs();
}

void EINK_send_data(unsigned char data) {
	EINK_set_data();
	SPI_unset_EINK_cs();
	SPI_send(data);
	SPI_set_EINK_cs();
}

void EINK_clear_frame() {
	EINK_send_command(DATA_START_TRANSMISSION_1);
	_delay_ms(2);

	for(int i = 0; i < WIDTH * HEIGHT / 8; i++) {
        EINK_send_data(0xFF);  
    } 
	_delay_ms(2);

	EINK_send_command(DATA_START_TRANSMISSION_2);
	_delay_ms(2);

	for(int i = 0; i < WIDTH * HEIGHT / 8; i++) {
        EINK_send_data(0xFF);  
    }  
	_delay_ms(2);
}

void EINK_init() {
	
	epdwidth = WIDTH;
	
	//RESET PIN
	DDRD |= (1 << DDD5);
	PORTD |= (1 << PD5);
	
	
	// Data/Command pin
	DDRD |= (1 << DDD4);
	
	EINK_send_command(BOOSTER_SOFT_START);
	EINK_send_data(0x17);
	EINK_send_data(0x17);
	EINK_send_data(0x17);
	EINK_send_command(POWER_ON);
	_delay_ms(2000);
	EINK_send_command(PANEL_SETTING);
	EINK_send_data(0x8F);
	EINK_send_data(0x8F);
	EINK_send_command(VCOM_AND_DATA_INTERVAL_SETTING);
	EINK_send_data(0x37);
	EINK_send_command(RESOLUTION_SETTING);
	EINK_send_data(0x68);
	EINK_send_data(0x00);
	EINK_send_data(0xD4);
	_delay_ms(2000);
	
}

void EINK_display_frame(unsigned char* frame_buffer_black, unsigned char* frame_buffer_red) {
	if (frame_buffer_black != NULL) {
        EINK_send_command(DATA_START_TRANSMISSION_1);
		_delay_ms(2);

        for (int i = 0; i <  epdwidth*HEIGHT/8; i++) {
            EINK_send_data(frame_buffer_black[i]);
        }
    }
	_delay_ms(2);

    if (frame_buffer_red != NULL) {
        EINK_send_command(DATA_START_TRANSMISSION_2);
		_delay_ms(2);

        for (int i = 0; i < epdwidth*HEIGHT/8; i++) {
            EINK_send_data(frame_buffer_red[i]);
        }
		_delay_ms(2);

    }
}

void EINK_draw_absolute_pixel(unsigned char* image, int x, int y, int colored) {
	if (x < 0 || x >= epdwidth || y < 0 || y >= HEIGHT) { // Partialite jaoks vaja partiali suurust! Muidu overflow
        return;
    }
	
    if (IF_INVERT_COLOR) {
        if (colored) {
            image[(x + y * epdwidth) / 8] |= 0x80 >> (x % 8);
        } else {
            image[(x + y * epdwidth) / 8 ] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            image[(x + y * epdwidth) / 8]  &= ~(0x80 >> (x % 8));
        } else {
            image[(x + y * epdwidth) / 8 ] |= 0x80 >> (x % 8);
        }
    }
}

void EINK_set_partial_black(unsigned char* buffer, int x, int y, int w, int l) {
	EINK_send_command(PARTIAL_IN);
	EINK_send_command(PARTIAL_WINDOW);

	EINK_send_data( x & 0xf8); // X on 8 kordne
	EINK_send_data(((x&0xf8) + w - 1) | 0x07);

	EINK_send_data(y >> 8);
	EINK_send_data(y & 0xff);
	EINK_send_data((y + l -1) >> 8);
	EINK_send_data((y + l - 1) & 0xff);
	EINK_send_data(0x01);
	_delay_ms(2);
	EINK_send_command(DATA_START_TRANSMISSION_1);
	if (buffer != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            EINK_send_data(buffer[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            EINK_send_data(0x00);  
        }  
    }
    _delay_ms(2);
	EINK_send_command(PARTIAL_OUT);  
}

void EINK_set_partial_red(unsigned char* buffer, int x, int y, int w, int l) {
	EINK_send_command(PARTIAL_IN);
	EINK_send_command(PARTIAL_WINDOW);

	EINK_send_data( x & 0xf8); // X on 8 kordne
	EINK_send_data(((x&0xf8) + w - 1) | 0x07);

	EINK_send_data(y >> 8);
	EINK_send_data(y & 0xff);
	EINK_send_data((y + l -1) >> 8);
	EINK_send_data((y + l - 1) & 0xff);
	EINK_send_data(0x01);
	_delay_ms(2);
	EINK_send_command(DATA_START_TRANSMISSION_2);
	if (buffer != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            EINK_send_data(buffer[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            EINK_send_data(0x00);  
        }  
    }
    _delay_ms(2);
	EINK_send_command(PARTIAL_OUT);  
}

void EINK_wait() {
    while(PIND & !(1<<PD3));
}
