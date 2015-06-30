/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "iic.h"
#include "pcf8591.h"


#define DEVADDR         0x90
#define DEVWRITE        0
#define DEVREAD         1


unsigned char pcf8591_adc(unsigned char dev)
{
    unsigned char c;

 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVREAD)) {
        goto start;
    }

    c = iic_recv(1);

    iic_stop();

    return c;
}

void pcf8591_adcstr(unsigned char dev, 
                    unsigned char __idata *p,
                    unsigned char n)
{
 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVREAD)) {
        goto start;
    }

    for (; n != 0; p++, n--) {
        *p = iic_recv(n == 1);
    }

    iic_stop();
}

unsigned char pcf8591_ctrl(unsigned char dev, unsigned char ctrl)
{
 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
        goto start;
    }

    if (iic_send(ctrl)) {
        goto start;
    }

    iic_stop();
}

void pcf8591_dac(unsigned char dev, unsigned char ctrl, unsigned char c)
{
 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
        goto start;
    }

    if (iic_send(PCF8591_OUTPUTENABLE | ctrl)) {
        goto start;
    }

    if (iic_send(c)) {
        goto start;
    }

    iic_stop();
}

void pcf8591_dacstr(unsigned char dev, 
                    unsigned char ctrl, 
                    const unsigned char __idata *p,
                    unsigned char n)
{
 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
        goto start;
    }

    if (iic_send(PCF8591_OUTPUTENABLE | ctrl)) {
        goto start;
    }

    for (; n != 0; p++, n--) {
        if (iic_send(*p)) {
            goto start;
        }
    }

    iic_stop();
}
