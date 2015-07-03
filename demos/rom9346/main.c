/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <rom9346.h>
#include <print.h>


#define OUTPUT(c)                                               \
    do {                                                        \
        if (sizeof(c) == 2) {                                   \
            UARTHEX4(c);                                        \
        } else {                                                \
            UARTHEX2(c);                                        \
        }                                                       \
        UARTCHAR(' ');                                          \
    } while (0)


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void)
{
    rom9346_word_t              c;
    rom9346_word_t __idata      p[10];
    unsigned char               i;

    welcome();

    rom9346_write_enable();

    rom9346_erase_all();

    for (i = 0; i < 64; i++) {
        rom9346_write(i, i + 0x10);
    }

    rom9346_write_disable();

    for (i = 0; i < 5; i++) {
        c = rom9346_read(i);
        OUTPUT(c);
    }
    
    rom9346_readstr(5, p, 5);
    for (i = 0; i < 5; i++) {
        OUTPUT(p[i]);
    }

    while (1);
}
