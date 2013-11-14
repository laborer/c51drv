/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __EEPROM_H
#define __EERPOM_H


void eeprom_erase(unsigned int addr);
void eeprom_write(unsigned int addr, unsigned char c);
unsigned char eeprom_read(unsigned int addr);


#endif /* __EEPROM_H */
