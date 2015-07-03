/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __DS1820_H
#define __DS1820_H


#include "common.h"


#define DS1820_DQ               P2_7

#define DS1820_ERR_INIT         -1
#define DS1820_ERR_CRC          -2
#define DS1820_ERR_SEARCH       -3
#define DS1820_ERR_TIMEOUT      -4


int8_t ds1820_read_rom(uint8_t __idata *rom);
int8_t ds1820_read_power_supply(uint8_t __idata *rom);
int8_t ds1820_convert_t(uint8_t __idata *rom);
int8_t ds1820_copy_scratchpad(uint8_t __idata *rom);
int8_t ds1820_recall_e2(uint8_t __idata *rom);
int8_t ds1820_search_rom(uint8_t __idata *rom);
int8_t ds1820_alarm_search(uint8_t __idata *rom);
int16_t ds1820_read_temperature(uint8_t __idata *rom);
int8_t ds1820_read_resolution(uint8_t __idata *rom);
int8_t ds1820_write_resolution(uint8_t __idata *rom, uint8_t res);
int8_t ds1820_read_triggers(uint8_t __idata *rom, 
                            uint8_t __idata *th,
                            uint8_t __idata *tl);
int8_t ds1820_write_triggers(uint8_t __idata *rom, 
                             uint8_t th,
                             uint8_t tl);
int16_t ds1820_measure_temperature(uint8_t __idata *rom);


#endif /* __DS1820_H */
