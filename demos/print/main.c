/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char i;

    welcome();

    while (1) {
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_UNSIGNED, 7, 52312);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_PLUS, 7, 52312);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_HEX, 7, 52312);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_HEX | PRINT_UPPERCASE, 7, 52312);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_SPACE | PRINT_LEFTALIGN, 7, 12345);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_int(uart_putchar, PRINT_ZERO, 7, 12345);
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_str(uart_putchar, 0, 10, "test");
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        i = print_str(uart_putchar, PRINT_LEFTALIGN, 10, "tset");
        UARTCHAR('_');
        UARTCHAR('0' + i);
        UARTCHAR('_');
        UARTCHAR('\n');
        delay_ms(1000);
    }
}
