/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __SPI_H
#define __SPI_H


#define SPI_SCLK        P2_2
#define SPI_MOSI        P2_1
#define SPI_MISO        P2_0


#define SPI_EXCH(c, n)                                          \
    spi_exchange((c) << (8 - (n)), (n))

#define SPI_SEND(c)                                             \
    spi_exchange((c), 8)

#define SPI_RECV() \
    spi_exchange(0, 8)


unsigned char spi_exchange(unsigned char c, unsigned char n);


#endif /* __SPI_H */
