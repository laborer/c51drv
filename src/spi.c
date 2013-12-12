/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "spi.h"


#define SCLK    SPI_SCLK
#define MOSI    SPI_MOSI
#define MISO    SPI_MISO

#define WAIT_READY()                                            \
    while (length) {}


#ifdef SPI_UART_RECV_ENABLE

static unsigned char __idata    *buffer;
static unsigned char            length;

/* UART service routine.  With SPI_UART_RECV_ENABLE, you can't use the
   uart module as it defines a UART service routine as well */
void spi_uart_interrupt(void) __interrupt SI0_VECTOR __using 1
{
    length -= 1;
    if (RI) {
        *buffer = REVERSE(SBUF);
        buffer += 1;
        if (length == 0) {
            REN = 0;
        }
        RI = 0;
    }
}

/* Initialize UART Mode 0 accelerated SPI */
void spi_uart_init(void)
{
    /* Use UART Mode 0 */
    /* SM0 SM1 SM2 REN TB8 RB8 TI RI
       0   0   0   0   0   0   0  0  */
    SCON = 0x00;

    /* Enable UART interrupt */
    ES = 1;
    /* Enable global interrupt */
    EA = 1;
}

/* Receive n bytes to buf using accelerated SPI */
void spi_uart_recvstr(unsigned char __idata *buf, unsigned char n)
{
    WAIT_READY();

    buffer = buf;
    length = n;
    SCLK = 1;
    REN = 1;
}

/* Receive a byte using accelerated SPI */
unsigned char spi_uart_recv(void)
{
    unsigned char __idata buf;

    spi_uart_recvstr(&buf, 1);

    WAIT_READY();
    
    return buf;
}

#endif /* SPI_UART_RECV_ENABLE */

/* Initialize SPI */
void spi_init(void)
{
    SCLK = 0;
    MISO = 1;
}

/* If SPI is currently sending or receiving data */
unsigned char spi_isbusy(void)
{
#ifdef SPI_UART_RECV_ENABLE
    return length;
#else
    return 0;
#endif
}

/* Send n bytes in buf */
void spi_sendstr(unsigned char __idata *buf, unsigned char n)
{
    for (; n != 0; buf++, n--) {
        spi_send(*buf);
    }
}

/* Receive n bytes to buf */
void spi_recvstr(unsigned char __idata *buf, unsigned char n)
{
    for (; n != 0; buf++, n--) {
        *buf = spi_recv();
    }
}

/* Send byte c */
void spi_send(unsigned char c) {
    unsigned char i;

#ifdef SPI_UART_RECV_ENABLE
    WAIT_READY();
#endif
    for (i = 8; i != 0; i--) {
        MOSI = c & 0x80;
        SCLK = 1;
        DELAY_US(1);
        c <<= 1;
        SCLK = 0;
        DELAY_US(1);
    }
}

/* Receive a byte */
unsigned char spi_recv(void) {
    unsigned char       c;
    unsigned char       i;

#ifdef SPI_UART_RECV_ENABLE
    WAIT_READY();
#endif
    MOSI = 0;
    for (i = 8; i != 0; i--) {
        SCLK = 1;
        DELAY_US(1);
        c = (c << 1) | MISO;
        SCLK = 0;
        DELAY_US(1);
    }

    return c;
}
