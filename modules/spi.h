/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __SPI_H
#define __SPI_H


#include "common.h"


#define SPI_SCLK        P2_2
#define SPI_MOSI        P2_1
#define SPI_MISO        P2_0

/* Enable accelerating SPI receiving speed using UART Mode 0 */
/* #define SPI_UART_RECV_ENABLE */


#if defined SDCC || defined __SDCC
#pragma callee_saves spi_init,spi_isbusy
#endif


void spi_init(void);
uint8_t spi_isbusy(void);
void spi_sendstr(uint8_t __idata *buf, uint8_t n);
void spi_recvstr(uint8_t __idata *buf, uint8_t n);
void spi_send(uint8_t c);
uint8_t spi_recv(void);

#ifdef SPI_UART_RECV_ENABLE

/* With UART Mode 0, SCLK has to be TXD pin and MISO has to be RXD
   pin */
#define SPI_SCLK        TXD
#define SPI_MISO        RXD

#if defined SDCC || defined __SDCC
void spi_uart_interrupt(void) __interrupt SI0_VECTOR __using 1;
#endif

void spi_uart_init(void);
uint8_t spi_uart_isbusy(void);
void spi_uart_recvstr(uint8_t __idata *buf, uint8_t n);
uint8_t spi_uart_recv(void);

#endif /* SPI_UART_RECV_ENABLE */


#endif /* __SPI_H */
