/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "rom9346.h"
#include "print.h"


#define OUTPUT(c)                                               \
    do {                                                        \
        if (sizeof(c) == 2) {                                   \
            UARTCHAR((c) >> 8);                                 \
        }                                                       \
        UARTCHAR(c);                                            \
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

    rom9346_ewen();

    rom9346_eral();

    for (i = 0; i < 16; i++) {
        rom9346_write(i, 'A' + i);
    }

    rom9346_ewds();

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
