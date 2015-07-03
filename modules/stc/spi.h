/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __SPI_H
#define __SPI_H


#include "../common.h"
#include "stc.h"


#define SPI_SYSCLK_4    0
#define SPI_SYSCLK_16   1
#define SPI_SYSCLK_64   2
#define SPI_SYSCLK_128  3

#define SPI_MSBFIRST    0
#define SPI_LSBFIRST    1

#define SPI_IDLELOW     0   
#define SPI_IDLEHIGH    1    

#define SPI_FIRSTEDGE   0
#define SPI_SECONDEDGE  1

#define SPI_DORD        SPI_MSBFIRST
#define SPI_CPOL        SPI_IDLELOW
#define SPI_CPHA        SPI_FIRSTEDGE
#define SPI_SPEED       SPI_SYSCLK_64


#if defined SDCC || defined __SDCC
void spi_interrupt(void) __interrupt 9 __using 1;
#endif

void spi_init(void);
uint8_t spi_isbusy(void);
void spi_sendstr(uint8_t __idata *buf, uint8_t n);
void spi_recvstr(uint8_t __idata *buf, uint8_t n);
void spi_send(uint8_t c);
uint8_t spi_recv(void);


#endif /* __SPI_H */
