/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "iic.h"
#include "rom2402.h"


#define PAGEMASK ((1 << ROM2402_PAGEBITS) - 1)


unsigned char rom2402_read(unsigned char dev, unsigned char addr)
{
    unsigned char c;

 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    iic_start();

    if (iic_send((dev << 1) & 0x0E | 0xA1)) {
        goto start;
    }

    c = iic_recv(1);

    iic_stop();

    return c;
}

void rom2402_readstr(unsigned char dev, 
                     unsigned char addr, 
                     unsigned char __idata *p,
                     unsigned char n)
{
    unsigned char i;

 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    iic_start();

    if (iic_send((dev << 1) & 0x0E | 0xA1)) {
        goto start;
    }

    i = (~addr & PAGEMASK) + 1;
    i = (i > n) ? n : i;
    n -= i;
    addr += i;
    if (addr == 0) {
        dev += 1;
    }

    for (; i != 0; p++, i--) {
        *p = iic_recv(i == 1);
    }

    iic_stop();

    if (n > 0) {
        goto start;
    }
}

void rom2402_write(unsigned char dev, unsigned char addr, unsigned char c)
{
 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    if (iic_send(c)) {
        goto start;
    }

    iic_stop();
}

void rom2402_writestr(unsigned char dev, 
                      unsigned char addr, 
                      const unsigned char __idata *p,
                      unsigned char n)
{
    unsigned char i;
    unsigned char k;

 start:
    iic_start();
    
    if (iic_send((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    i = (~addr & PAGEMASK) + 1;
    i = (i > n) ? n : i;
    k = i;

    for (; i != 0; p++, i--) {
        if (iic_send(*p)) {
            p -= k - i;
            goto start;
        }
    }

    n -= k;
    addr += k;
    if (addr == 0) {
        dev += 1;
    }

    iic_stop();

    if (n > 0) {
        goto start;
    }
}
