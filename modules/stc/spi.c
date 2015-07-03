/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "../common.h"
#include "stc.h"
#include "spi.h"


#define SEND    0
#define RECV    1


static uint8_t __idata  *buffer;
static uint8_t          length;
static __bit            direction;


#ifdef SPI_CALLBACK
extern void SPI_CALLBACK(uint8_t c) __using 1;
#endif /* SPI_CALLBACK */

void spi_interrupt(void) __interrupt 9 __using 1
{
    /* Clear SPI flags */
    /* SPIF WCOL - - - - - - */
    SPSTAT = 0x80 | 0x40;

    length -= 1;
    if (direction == SEND) {
        if (length != 0) {
            buffer += 1;
            SPDAT = *buffer;
        }
    } else {
        *buffer = SPDAT;
#ifdef SPI_CALLBACK
        SPI_CALLBACK(*buffer);
#endif /* SPI_CALLBACK */
        buffer += 1;
        if (length != 0) {
            SPDAT = 0;
        }
    }
}

void spi_init(void)
{
    /* Clear SPI flags */
    /* SPIF WCOL - - - - - - */
    SPSTAT = 0x80 | 0x40;

    /* Set SPI Control Register */
    /* SSIG SPEN DORD MSTR CPOL CPHA SPR1 SPR0 */
    SPCTL = (0x80 | 0x40 | (SPI_DORD ? 0x20 : 0) | 0x10 
             | (SPI_CPOL ? 0x08 : 0) | (SPI_CPHA ? 0x04 : 0) | SPI_SPEED);

    /* Enable SPI interrupt */
    /* - - - - - - ESPI ES2 */
    IE2 |= 0x02;

    /* Enable global interrupt */
    EA = 1;

    length = 0;
}

uint8_t spi_isbusy(void)
{
    return length;
}

void spi_sendstr(uint8_t __idata *buf, uint8_t n)
{
    while (length);

    buffer = buf;
    length = n;
    direction = SEND;

    SPDAT = *buf;
}

void spi_recvstr(uint8_t __idata *buf, uint8_t n)
{
    while (length);

    buffer = buf;
    length = n;
    direction = RECV;

    SPDAT = 0;
}

void spi_send(uint8_t c) {
    uint8_t __idata buf;

    buf = c;
    spi_sendstr(&buf, 1);
}

uint8_t spi_recv(void) {
    uint8_t __idata buf;

    spi_recvstr(&buf, 1);

    while (length);
    
    return buf;
}
