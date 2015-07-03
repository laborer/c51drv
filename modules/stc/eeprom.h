/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __EEPROM_H
#define __EERPOM_H


#include "../common.h"


void eeprom_erase(uint16_t addr);
void eeprom_write(uint16_t addr, uint8_t c);
uint8_t eeprom_read(uint16_t addr);


#endif /* __EEPROM_H */
