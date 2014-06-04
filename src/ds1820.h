/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __DS1820_H
#define __DS1820_H


#define DS1820_DQ               P2_7

#define DS1820_ERR_INIT         -1
#define DS1820_ERR_CRC          -2
#define DS1820_ERR_SEARCH       -3
#define DS1820_ERR_TIMEOUT      -4


char ds1820_read_rom(unsigned char __idata *rom);
char ds1820_read_power_supply(unsigned char __idata *rom);
char ds1820_convert_t(unsigned char __idata *rom);
char ds1820_copy_scratchpad(unsigned char __idata *rom);
char ds1820_recall_e2(unsigned char __idata *rom);
char ds1820_search_rom(unsigned char __idata *rom);
char ds1820_alarm_search(unsigned char __idata *rom);
int ds1820_read_temperature(unsigned char __idata *rom);
char ds1820_read_resolution(unsigned char __idata *rom);
char ds1820_write_resolution(unsigned char __idata *rom, unsigned char res);
char ds1820_read_triggers(unsigned char __idata *rom, 
                          unsigned char __idata *th,
                          unsigned char __idata *tl);
char ds1820_write_triggers(unsigned char __idata *rom, 
                           unsigned char th,
                           unsigned char tl);
int ds1820_measure_temperature(unsigned char __idata *rom);


#endif /* __DS1820_H */
