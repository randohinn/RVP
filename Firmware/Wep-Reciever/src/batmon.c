#include <include/batmon.h>
#include <include/i2c.h>
#include <avr/io.h>
#include <util/delay.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvıtjamooduli akuhalduse setup & SOC lugemine */


uint8_t batmon_init() {
	DDRD &= ~(1 << DDD0);
	DDRD &= ~(1 << DDD1);

	i2c_init(20000L);

	uint8_t subCommandMSB = (BQ27441_CONTROL_DEVICE_TYPE >> 8);
	uint8_t subCommandLSB = (BQ27441_CONTROL_DEVICE_TYPE & 0x00FF);
	uint8_t command[2] = {subCommandLSB, subCommandMSB};
	uint8_t data[2] = {0, 0};
	
	uint8_t status = i2c_start(0xAA);
	if(!status) {
		i2c_write(0x00);
		i2c_write(command[0]);
		i2c_write(command[1]);
		i2c_stop();

		i2c_read_multiple(0xAA,0xAB,0x00,2,0,data);

		uint16_t devt = ((uint16_t)data[1] << 8) | data[0];
		if(devt == 0x0421) {
			return 1;
		} 
	} 
	return 0;
}

uint8_t batmon_config(uint16_t dcap) {
	uint8_t status = i2c_start(0xAA);
	if(!status) {
        // UNSEAL IC
		i2c_write(0x00);
		i2c_write(BQ27441_UNSEAL_KEY & 0x00FF);
		i2c_write((BQ27441_UNSEAL_KEY & 0xFF00) >> 8);
		i2c_write(0x00);
		i2c_write(BQ27441_UNSEAL_KEY& 0x00FF);
		i2c_write((BQ27441_UNSEAL_KEY & 0xFF00) >> 8);
		i2c_stop();
        
		uint8_t stat = i2c_start(0xAA);
		if(!stat) {
        
            // L‰heme CFGUPDATE OLEKUSSE
			i2c_write(0x00);
			i2c_write(0x13);
			i2c_write(0x00);
            i2c_stop();

            uint8_t data[2] = {0, 0};
            _delay_ms(1000);

            i2c_read_multiple(0xAA,0xAB,0x06,2,0,data);
            if(data[0] & (1 << 4)) {
                // Enable block data memory control
                i2c_start(0xAA);
                i2c_write(0x61);
                i2c_write(0x00);
                i2c_stop();
                
                // Ligip‰‰s Design Capacity'le
                i2c_start(0xAA);
                i2c_write(0x3E);
                i2c_write(0x52);
                i2c_stop();
                
                // Kirjutame offseti
                i2c_start(0xAA);
                i2c_write(0x3F);
                i2c_write(0x00);
                i2c_stop();
                
                // Loeme algse checksum'i (TI dokumentatsioon natuke niru siin)
                uint8_t cs = i2c_read_single(0xAA,0xAB,0x60,0);

                if(data[1] == 0xAC) {
                    // Loeme design capacity, kirjutame ¸le
                    data[0] = i2c_read_single(0xAA,0xAB,0x4A,0);
                    data[1] = i2c_read_single(0xAA,0xAB,0x4B,0);  
                    i2c_start(0xAA);
                    i2c_write(0x4a);
                    i2c_write((dcap & 0xFF00)>>8);
                    i2c_stop();
                    i2c_start(0xAA);
                    i2c_write(0x4b);
                    i2c_write(dcap & 0x00FF);
                    i2c_stop();
                    // Arvutame uue checksum'i ja kirjutame selle. valem ptk 3.1 https://www.ti.com/lit/ug/sluuac9a/sluuac9a.pdf
                    uint8_t temp = (255 - cs - data[0] - data[1]) % 256;
                    uint8_t newcs = 255- ((temp+((dcap & 0xFF00)>>8)+(dcap & 0x00FF)) % 256);
                    i2c_start(0xAA);
                    i2c_write(0x60);
                    i2c_write(newcs);
                    i2c_stop();
                    // Soft reset & seal
                    i2c_start(0xAA);
                    i2c_write(0x00);
                    i2c_write(0x42);
                    i2c_write(0x00);
                    i2c_stop();
                    i2c_start(0xAA);
                    i2c_write(0x00);
                    i2c_write(0x20);
                    i2c_write(0x00);
                    i2c_stop();

                }

            }
		}
	}
	return 1;
}

uint8_t batmon_soc() {
    // Loeme state of charge v‰‰rtuse
    return i2c_read_single(0xAA,0xAB,0x1C,0);
}