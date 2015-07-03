/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __PCF8591_H
#define __PCF8591_H


#include "common.h"


#define PCF8591_INPUTCONFIG0    0x00
#define PCF8591_INPUTCONFIG1    0x10
#define PCF8591_INPUTCONFIG2    0x20
#define PCF8591_INPUTCONFIG3    0x30
#define PCF8591_AUTOINCREMENT   0x04
#define PCF8591_OUTPUTENABLE    0x40


uint8_t pcf8591_adc(uint8_t dev);
void pcf8591_adcstr(uint8_t dev, 
                    uint8_t __idata *p,
                    uint8_t n);
uint8_t pcf8591_ctrl(uint8_t dev, uint8_t ctrl);
void pcf8591_dac(uint8_t dev, uint8_t ctrl, uint8_t c);
void pcf8591_dacstr(uint8_t dev, 
                    uint8_t ctrl, 
                    const uint8_t __idata *p,
                    uint8_t n);


#endif /* __PCF8591_H */
