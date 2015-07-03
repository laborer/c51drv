/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "iic.h"
#include "rom2402.h"


#define DEVADDR         0xA0
#define DEVWRITE        0
#define DEVREAD         1
#define PAGEMASK        ((1 << ROM2402_PAGEBITS) - 1)


uint8_t rom2402_read(uint8_t dev, uint8_t addr)
{
    uint8_t c;

 start:
    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVREAD)) {
        goto start;
    }

    c = iic_recv(1);

    iic_stop();

    return c;
}

void rom2402_readstr(uint8_t dev,
                     uint8_t addr,
                     uint8_t __idata *p,
                     uint8_t n)
{
    uint8_t i;

 start:
    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
        goto start;
    }

    if (iic_send(addr)) {
        goto start;
    }

    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVREAD)) {
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

void rom2402_write(uint8_t dev, uint8_t addr, uint8_t c)
{
 start:
    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
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

void rom2402_writestr(uint8_t dev,
                      uint8_t addr,
                      const uint8_t __idata *p,
                      uint8_t n)
{
    uint8_t     i;
    uint8_t     k;

 start:
    iic_start();

    if (iic_send((dev << 1) & 0x0E | DEVADDR | DEVWRITE)) {
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
