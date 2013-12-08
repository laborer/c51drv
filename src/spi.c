/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "spi.h"


#define SCLK    SPI_SCLK
#define MOSI    SPI_MOSI
#define MISO    SPI_MISO


void spi_init()
{
    SCLK = 0;
    MISO = 1;
}

unsigned char spi_isbusy()
{
    return 0;
}

void spi_sendstr(unsigned char __idata *buf, unsigned char n)
{
    for (; n != 0; buf++, n--) {
        spi_send(*buf);
    }
}

void spi_recvstr(unsigned char __idata *buf, unsigned char n)
{
    for (; n != 0; buf++, n--) {
        *buf = spi_recv();
    }
}

void spi_send(unsigned char c) {
    unsigned char i;

    for (i = 8; i != 0; i--) {
        SCLK = 0;
        DELAY_US(1);
        MOSI = c & 0x80;
        SCLK = 1;
        DELAY_US(1);
        c <<= 1;
    }
}

unsigned char spi_recv() {
    unsigned char       c;
    unsigned char       i;

    MOSI = 0;
    for (i = 8; i != 0; i--) {
        SCLK = 0;
        DELAY_US(1);
        SCLK = 1;
        DELAY_US(1);
        c = (c << 1) | MISO;
    }

    return c;
}
