/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <lcd1602.h>
#include <print.h>
#include <spi.h>


#define CS P3_4


#define LCDCHAR(c)                                              \
    lcd1602_putchar(c);

#define LCDSTR(str)                                             \
    print_str(lcd1602_putchar, 0, 0, str)

#define LCDINT4(num)                                            \
    print_int(lcd1602_putchar, 0, 4, num)

#define LCDUINT(num)                                            \
    print_int(lcd1602_putchar, PRINT_UNSIGNED, 0, num)

#define LCDHEX2(num)                                            \
    print_int(lcd1602_putchar,                                  \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              2,                                                \
              num)

#define LCDHEX4(num)                                            \
    print_int(lcd1602_putchar,                                  \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              4,                                                \
              num)


unsigned int read_9346(unsigned char addr)
{
    unsigned int c;

    CS = 0;
    DELAY_US(1);
    spi_init();

    CS = 1;
    while (!spi_recv());

    spi_send(0x01);
    spi_send(0x80 | addr);

#ifdef SPI_UART_RECV_ENABLE
    c = (spi_uart_recv() << 8) | spi_uart_recv();
#else
    c = (spi_recv() << 8) | spi_recv();
#endif

    CS = 0;

    return c;
}

void readstr_9346(unsigned char addr, 
                  unsigned char __idata *buf, 
                  unsigned char n)
{
    CS = 0;
    DELAY_US(1);
    spi_init();

    CS = 1;
    while (!spi_recv());

    spi_send(0x01);
    spi_send(0x80 | addr);

#ifdef SPI_UART_RECV_ENABLE
    spi_uart_recvstr(buf, n);
    while (spi_uart_isbusy());
#else 
    spi_recvstr(buf, n);
#endif
    
    CS = 0;
}

void welcome(void)
{
    delay_ms(100);
    lcd1602_init();
    LCDSTR("c51drv");
}

void main(void) {
    unsigned char               i;
    unsigned char __idata       buf[8];

    welcome();

    LCD1602_POSITION(0, 1);

#ifdef SPI_UART_RECV_ENABLE
    spi_uart_init();
#endif
    
    /* for (i = 0; i < 4; i++) { */
    /*     LCDHEX4(read_9346(i+2)); */
    /* } */

    readstr_9346(4, buf, 8);

    for (i = 0; i < 8; i++) {
        LCDHEX2(buf[i]);
    }

    while (1);
}
