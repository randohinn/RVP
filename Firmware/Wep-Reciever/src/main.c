#include <avr/io.h>
#include <include/spi.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <include/nrf24l01.h>
#include <avr/interrupt.h>
#include <include/eink.h>
#include <include/batmon.h>
#include <avr/sleep.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli peakood */

volatile uint8_t data[10];
volatile uint8_t data_available = 0;
uint16_t tc = 0;

ISR(INT6_vect) {
    if(data_available == 0) {
        nrf24l01_read_register(R_RX_PAYLOAD, (uint8_t*) data,10);
        data_available = 1;
    }
}

// Opereerib etteantaval eraldiseisval puhvril, seetõttu ei paikne EINK libraga koos. 
void clean_buffer(uint8_t* buffer) {
    for(uint8_t x = 0; x < epdwidth; x++) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
            EINK_draw_absolute_pixel(buffer,x,y,0);
        }
    }
    for(int i = 0; i < 122/epdwidth; i++) {
        for(int j = 0; j < 250/HEIGHT; j++) {
            EINK_set_partial_red(buffer, i*epdwidth,j*HEIGHT, epdwidth,HEIGHT);
            EINK_set_partial_black(buffer, i*epdwidth,j*HEIGHT, epdwidth,HEIGHT);
        }
    }
}


ISR(TIMER1_COMPA_vect) {
    tc++;
    if(tc == 3600000) { // countime tunni
        if(batmon_soc() < 25) {
            cli();
            unsigned char buffer[(epdwidth / 8) * HEIGHT];
            for(uint8_t x = 0; x < epdwidth; x++) {
                for (uint8_t y = 0; y < HEIGHT; y++) {
                    EINK_draw_absolute_pixel(buffer,x,y,0);
                }
            }
            for(int i = 0; i < 122/epdwidth; i++) {
                for(int j = 0; j < 250/HEIGHT; j++) {
                    EINK_set_partial_red(buffer, i*epdwidth,j*HEIGHT, epdwidth,HEIGHT);
                    EINK_set_partial_black(buffer, i*epdwidth,j*HEIGHT, epdwidth,HEIGHT);
                }
            }
            uint8_t cv[1];
            //Overwrite with no powerup set
            cv[0] = (1 << CRCO) | (1 << EN_CRC) | (1 << MASK_MAX_RT);
            nrf24l01_write_register(CONFIG, cv,1);
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();
        }
        tc = 0;
    }
}

int main() {

    //DEBUG led
    DDRF = (1 << DDF0);
	
    if(batmon_init()) {
        batmon_config(700);
        if(batmon_soc() > 25) { // Akut üle 25%? Läheme käima
        
            // Ja tekitame timeri, millega akut kontrollida 100ms
            TIMSK1 |= (1 << OCIE1A);
            TCCR1B |= (1 << WGM12) | (1<<CS10) | (1 << CS12);
            OCR1A = 1562;
        
            //IRQ
            DDRE &= ~(1 << DDE6);
            EICRB |= (1<<ISC60)|(0<<ISC61);   //INT6 falling edge
            EIMSK |= (1<<INT6);
            sei();
            
            // EINK Busy
            DDRD &= ~(1 << DDD3);

            SPI_init();   
            EINK_init();
            EINK_wait();
            
            uint8_t buffer[(epdwidth / 8) * HEIGHT];
            clean_buffer(buffer);
            EINK_wait();
            
            // Valgeks
            EINK_send_command(DISPLAY_REFRESH);
            EINK_wait();

            // Delay piisavalt, et esimese refreshi jõuaks enne ära teha
            _delay_ms(15000);

            //Suvaline aadress
            uint8_t address[5];
            for(int i = 0; i < 5; i++) {
                address[i] = 0x12+i;
            }
                
            nrf24l01_init(5, address, 110, 10 ,MODE_RECIEVE);
            nrf24l01_read_register(R_RX_PAYLOAD,(uint8_t *)data,10); // Esimesel korral peame puhvritühjendust ise tegema..
            nrf24l01_reset_interrupts();    

            while(1) {
                if(data_available) {
                    if(data[0] == 'D') {
                        if(data[1] == 'B') {
                            EINK_set_partial_black(buffer, (uint8_t)data[2], (uint8_t)data[3], epdwidth,HEIGHT);
                            EINK_wait();
                            for(uint8_t x = 0; x < epdwidth; x++) {
                                for (uint8_t y = 0; y < HEIGHT; y++) {
                                    EINK_draw_absolute_pixel(buffer,x,y,0);
                                }
                            }
                        } else if (data[1] == 'R') {
                            EINK_set_partial_red(buffer, (uint8_t)data[2], (uint8_t)data[3], epdwidth,HEIGHT);
                            EINK_wait();
                            for(uint8_t x = 0; x < epdwidth; x++) {
                                for (uint8_t y = 0; y < HEIGHT; y++) {
                                    EINK_draw_absolute_pixel(buffer,x,y,0);
                                }
                            }
                        }
                    } else {
                        if(data[0] == 'S') {
                            EINK_draw_absolute_pixel(buffer,(uint8_t)data[1],(uint8_t)data[2],1);
                        } else if(data[0] == 'C') {
                            EINK_draw_absolute_pixel(buffer,data[1],data[2],0);
                        } else if(data[0] == 'R') {
                            EINK_send_command(DISPLAY_REFRESH);
                            EINK_wait();
                        } else if(data[0] == 'W') {
                            clean_buffer(buffer);
                            EINK_wait();
                        }
                    }
                        
                    nrf24l01_reset_interrupts();
                    data_available = 0;
                }
            }
        }
    }
    return 0;
}