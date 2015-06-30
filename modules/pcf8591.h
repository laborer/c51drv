/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __PCF8591_H
#define __PCF8591_H


#define PCF8591_INPUTCONFIG0    0x00
#define PCF8591_INPUTCONFIG1    0x10
#define PCF8591_INPUTCONFIG2    0x20
#define PCF8591_INPUTCONFIG3    0x30
#define PCF8591_AUTOINCREMENT   0x04
#define PCF8591_OUTPUTENABLE    0x40


unsigned char pcf8591_adc(unsigned char dev);
void pcf8591_adcstr(unsigned char dev, 
                    unsigned char __idata *p,
                    unsigned char n);
unsigned char pcf8591_ctrl(unsigned char dev, unsigned char ctrl);
void pcf8591_dac(unsigned char dev, unsigned char ctrl, unsigned char c);
void pcf8591_dacstr(unsigned char dev, 
                    unsigned char ctrl, 
                    const unsigned char __idata *p,
                    unsigned char n);


#endif /* __PCF8591_H */
