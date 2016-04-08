/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <stc/stc.h>
#include <stc/spi.h>


#define SCLK    P1_7
#define MOSI    P1_5
#define MISO    P1_6
#define SS      P1_4


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void)
{
    unsigned char __idata       buf[10];
    unsigned char               i;

    welcome();

    spi_init();

    while (1) {
        /* The following segment reads 10 bytes from a 93C46 */
        MISO = 1;
        SS = 1;

        while (!MISO);

        /* Send one bite (the start bit) to 93C46.  Since the SPI
           function on an STC MCU can only send one byte a time rather
           than an arbitrary number of bits, we have to stop SPI and
           send the bit manually like the spi module in ../spi.c.
           This method however doesn't seem to work if it's not placed
           at the beginning of an SPI conversation.  For example, if
           we call spi_send(0xC0) first, then turn off and on SPI
           using SPCTL will send an unwanted 1 to 93C46.  */
        SPCTL &= ~0x40;
        SCLK = 0;
        DELAY_US(1);
        MOSI = 1;
        SCLK = 1;
        DELAY_US(1);
        SPCTL |= 0x40;

        spi_send(0x80);
        while (spi_isbusy());

        spi_recvstr(buf, 10);
        while (spi_isbusy());

        SS = 0;

        for (i = 0; i < 10; i++) {
            UARTHEX2(buf[i]);
            UARTCHAR(' ');
        }
        UARTCHAR('\n');

        delay_ms(1000);
    }
}
