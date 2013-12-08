/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __SPI_H
#define __SPI_H


#define SPI_SCLK        P2_2
#define SPI_MOSI        P2_1
#define SPI_MISO        P2_0


#if defined SDCC || defined __SDCC
#pragma callee_saves spi_init,spi_isbusy
#endif /* SDCC */


void spi_init();
unsigned char spi_isbusy();
void spi_sendstr(unsigned char __idata *buf, unsigned char n);
void spi_recvstr(unsigned char __idata *buf, unsigned char n);
void spi_send(unsigned char c);
unsigned char spi_recv();


#endif /* __SPI_H */
