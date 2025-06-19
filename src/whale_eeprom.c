#include "rp2x_cat24m01.h"
#include "whale_eeprom.h"

uint w_eeprom_index;

//just the i2c inits
int w_eeprom_init(void) {
	
	int status = W_EEPROM_ERR;

	if(!i2c_init(W_I2C_INST, 400 * 1000)) return status;
	else status = W_EEPROM_OK;
	gpio_set_function(W_SCL_PIN, GPIO_FUNC_I2C);
	gpio_set_function(W_SDA_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(W_SCL_PIN);
	gpio_pull_up(W_SDA_PIN);

	w_eeprom_index = I2C_NUM(W_I2C_INST);

	return status;
}

//writes all 0xFFs! just like the device ships with :)
int w_eeprom_full_erase(void) {

	int status = W_EEPROM_ERR;

	uint8_t buf[256] = {0xFF};

	for(int i = 0; i < 512; i++) {
		
		if(!cat24_write_page(w_eeprom_index, W_EEPROM_SEL, i, 0x00, buf, 256)) {
			status = W_EEPROM_ERR;
			break;
		}

		status = w_eeprom_is_busy_timeout(4);
		if(status != W_EEPROM_OK) break;
	}

	return status;
}

//writes a string of up to length 256
int w_eeprom_write(int page, uint8_t addr, uint8_t *src, int size) {
	
	int status = W_EEPROM_ERR;
	
	if(page > 511) return status;
	if(addr > 255) return status;
	if(size > 255) return status;

	if(w_eeprom_is_busy_timeout(4) != W_EEPROM_OK) return status;
	if(cat24_write_page(w_eeprom_index, W_EEPROM_SEL, page, addr, src, size))
		status = W_EEPROM_OK;

	return status;
}

//
int w_eeprom_read(int page, uint8_t addr, uint8_t *dst, int size) {
	
	int status = W_EEPROM_ERR;

	if(page > 511) return status;
	if(addr > 255) return status;
	if(size > 255) return status;

	if(w_eeprom_is_busy_timeout(4) != W_EEPROM_OK) return status;
	if(cat24_read_page(w_eeprom_index, W_EEPROM_SEL, page, addr, dst, size))
		status = W_EEPROM_OK;

	return status;
}

// loop through for some amount of ms waiting for a write cyclre to end
// turns out it takes 4 ms to write a whole page
int w_eeprom_is_busy_timeout(uint ms) {
	
	int status = W_EEPROM_ERR;
	
	for(uint i = 0; (i < ms) && (status == W_EEPROM_ERR); i++) {
		if(cat24_is_busy(w_eeprom_index, W_EEPROM_SEL))
				status = W_EEPROM_ERR;	
		else status = W_EEPROM_OK;
		sleep_ms(1);
	}

	return status;
}
