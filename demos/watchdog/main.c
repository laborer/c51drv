/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */

/* This program pulls POWERBTN pin if it does not receive '@c|' from
   serial port every TIME_KICK seconds.
 */

#include <uart.h>
#include <print.h>


#define TIME_S(t)                                               \
    ((unsigned int)((t) / 1.0 * FOSC / 12 / 65536))

#define POWERBTN        P2_0
#define TIME_BOOT       TIME_S(60 * 5)
#define TIME_KICK       TIME_S(60 * 2.5)
    

unsigned int time;

void timer0int(void) __interrupt TF0_VECTOR __using 1
{
    time -= 1;
    if (!time) {
        POWERBTN = 1;
    }
}

void main(void) {
    const char __code   start[] = {'@','c','|'};
    unsigned char       c;
    unsigned char       i;

    POWERBTN = 0;

    time = TIME_BOOT;
    TMOD &= ~T0_MASK;
    TMOD |= T0_M0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;

    while (uart_baudrate_auto());

    uart_init();
    UARTSTR("watchdog\n");

    i = 0;
    while (1) {
        c = uart_getchar();

        if (c == start[i]) {
            i += 1;
            if (i == sizeof(start) / sizeof(start[0])) {
                i = 0;
                time = TIME_KICK;
                UARTCHAR('k');
                UARTCHAR(' ');
            }
        } else if (c == start[0]) {
            i = 1;
        } else {
            i = 0;
        }

        if (c == 't') {
            UARTUINT(time);
            UARTCHAR(' ');
        }
    }
}
