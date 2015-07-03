/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __DS1302_H
#define __DS1302_H


#include "common.h"


#define DS1302_CE       P3_5
#define DS1302_SCLK     P3_6
#define DS1302_IO       P3_4

#define DS1302_SECOND   0
#define DS1302_MINUTE   1
#define DS1302_HOUR     2
#define DS1302_DATE     3
#define DS1302_MONTH    4
#define DS1302_DAY      5
#define DS1302_YEAR     6


#define DS1302_CLOCKHALT_ENABLE()                               \
    ds1302_write(0, 0x80)

#define DS1302_CLOCKHALT_DISABLE()                              \
    ds1302_write(0, 0x00)

#define DS1302_CLOCKHALT_STATUS()                               \
    (ds1302_read(0) & 0x80) 

#define DS1302_WRITEPROTECT_ENABLE()                            \
    ds1302_write(7, 0x80)

#define DS1302_WRITEPROTECT_DISABLE()                           \
    ds1302_write(7, 0x00)

#define DS1302_WRITEPROTECT_STATUS()                            \
    ds1302_read(7)


uint8_t ds1302_read(uint8_t addr);
void ds1302_read_clock(uint8_t __idata *p);
void ds1302_write(uint8_t addr, uint8_t c);
void ds1302_write_clock(uint8_t __idata *p);


#endif /* __DS1302_H */
