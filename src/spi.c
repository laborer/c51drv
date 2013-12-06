/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "spi.h"


#define SCLK    SPI_SCLK
#define MOSI    SPI_MOSI
#define MISO    SPI_MISO


/* Shift out the first n bits of c and return the n bits shifted in
   during the same time. */
unsigned char spi_exchange(unsigned char c, unsigned char n)
{
    do {
        SCLK = 0;
        DELAY_US(1);
        MOSI = c & 0x80;
        SCLK = 1;
        DELAY_US(1);
        c = (c << 1) | MISO;
    } while (--n);

    return c;
}
