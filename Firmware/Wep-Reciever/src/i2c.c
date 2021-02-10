#include <include/i2c.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, i2C libra */


// Ootame kuni siinil on suhtlus toimunud
void _wait_transmission_complete() {
	while (!(TWCR & (1<<TWINT)));
}

// Käivitame I2C
void i2c_init(unsigned long clk_hz) {
	TWSR = 0;
	TWBR = ((F_CPU/clk_hz)-16)/2;
	TWCR = (1<<TWEN);
}

// Tõstatame STOP ja ootame siini vabastamist
void i2c_stop() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(TWCR & (1<<TWSTO));
}

// Algatame TWI operatsiooni ja kirjutame seadme aadressi siinile
uint8_t i2c_start(uint8_t address) {
	uint8_t status;
	
	// Saadame TWI stardi ja ootame selle toimumist.
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	_wait_transmission_complete();
	
	// Loeme TWI staatusest vajaminevad bitid.
	status = TW_STATUS & 0xF8;
	// Kui staatuse kood ei ole START või REP_START
	if((status != TW_START) && (status != TW_REP_START)) {
		return status; // Varajane return
	}
	
	// Kirjutame seadme aadressi siini ja ootame 
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	_wait_transmission_complete();
	
	// Loeme TWI staatusest vajaminevad bitid.
	status = TW_STATUS & 0xF8;
	// Kui staatus ei ole seadmelt tagasi tulnud ACK
	if((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK)) {
		i2c_stop();
		return status; // Varajane return
	}
	
	return 0; // Probleeme pole return

}

// Kirjutame siinile andmeid
uint8_t i2c_write(uint8_t data) {
	uint8_t status;
	
	// Kirjutame andmed siinile ja ootame operatsiooni lõppu
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	_wait_transmission_complete();
	
	// Loeme TWI staatusest vajaminevad bitid.
	status = TW_STATUS & 0xF8;
	// Kui seade ei tagastanud ACK
	if(status != TW_MT_DATA_ACK) {
		return status; // Varajane return
	}
	
	return 0; // Probleeme pole return
}

// Loeme ilma mikrokontrolleripoolset ACK'i tagastamata - tüüpiliselt viimase (või ainsa ühekordse) baidi lugemiseks
uint8_t i2c_read_nack() {
	
	TWCR = (1<<TWINT)|(1<<TWEN);
	_wait_transmission_complete();
	
	return TWDR;
}

// Loeme ilma mikrokontrolleripoolset ACK'i tagastades - tüüpiliselt annab seadmele märku, et prosele võib saata veel baite
uint8_t i2c_read_ack() {
	
	TWCR = (1<<TWINT)|(1<<TWEN) | (1 << TWEA);
	_wait_transmission_complete();
	
	return TWDR;
}

// Loeb siinilt andmeid seadme mingist registrist n_bytes jagu ja kirjutab result'i. ACK jälgib spec'i, kus viimast baiti ei ACK'ita, repstart määrab, kas vahepeal tekitatakse stop või mitte. 
void i2c_read_multiple(uint8_t write_address, uint8_t read_address, uint8_t register_address, uint8_t n_bytes, uint8_t repstart, uint8_t * result) {	
	uint8_t status = i2c_start(write_address);
	if(!status) {
		status = i2c_write(register_address);
		if(!status) {
			if(!repstart) {
				i2c_stop();
			}
			status = i2c_start(read_address);
			for(uint8_t i = 0; i < n_bytes; i++) {
				if(i == n_bytes-1) {
					result[i] = i2c_read_nack();
				} else {
					result[i] = i2c_read_ack();
				}
			}
			i2c_stop();
		}	
	} else {
		i2c_stop();
	}
}

// Loeme siinile üksiku baidi andmeid seadme mingist registrist. Ack parameetriga saab seadistada, kas ack tekitatakse või mitte. 
uint8_t i2c_read_single(uint8_t write_address, uint8_t read_address, uint8_t register_address, uint8_t ack) {
	uint8_t status = i2c_start(write_address);
	if(!status) {
		status = i2c_write(register_address);
		if(! status) {
			i2c_stop();
			
			status = i2c_start(read_address);
			if(!status) {
				uint8_t reading;
				if(ack) {
					reading = i2c_read_ack();
				} else {
					reading = i2c_read_nack();
				}
				i2c_stop();
				return reading;
			}
			
		} else {
			i2c_stop();
		}
		
	} else {
		i2c_stop();
	}
	return status;
}
