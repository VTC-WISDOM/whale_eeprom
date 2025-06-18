#include "rp2x_cat24m01.h"
#include "whale_eeprom.h"

int w_eeprom_init(void) {
	
}


int w_eeprom_full_erase(void);

int w_eeprom_write(int page, uint8_t addr, uint8_t *src, int size);
int w_eeprom_read(int page, uint8_t addr, uint8_t *dst, int size);
