/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __PCF8591_H
#define __PCF8591_H


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
