/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __SPI_H
#define __SPI_H


#define SPI_SCLK        P2_2
#define SPI_MOSI        P2_1
#define SPI_MISO        P2_0

/* Enable accelerating SPI receiving speed using UART Mode 0 */
/* #define SPI_UART_RECV_ENABLE */


#if defined SDCC || defined __SDCC
#pragma callee_saves spi_init,spi_isbusy
#endif


void spi_init();
unsigned char spi_isbusy();
void spi_sendstr(unsigned char __idata *buf, unsigned char n);
void spi_recvstr(unsigned char __idata *buf, unsigned char n);
void spi_send(unsigned char c);
unsigned char spi_recv();

#ifdef SPI_UART_RECV_ENABLE

/* With UART Mode 0, SCLK has to be TXD pin and MISO has to be RXD
   pin */
#define SPI_SCLK        TXD
#define SPI_MISO        RXD

#if defined SDCC || defined __SDCC
void spi_uart_interrupt(void) __interrupt SI0_VECTOR __using 1;
#endif

void spi_uart_init();
unsigned char spi_uart_isbusy();
void spi_uart_recvstr(unsigned char __idata *buf, unsigned char n);
unsigned char spi_uart_recv();

#endif /* SPI_UART_RECV_ENABLE */


#endif /* __SPI_H */
