/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <lcd1602.h>
#include "memory.h"


#define ESCAPE          0x1B
#define STATE_FAIL      0xFF
#define COMMAND_WAIT    0
#define COMMAND_B       1
#define COMMAND_S       2
#define COMMAND_C       3
#define COMMAND_P       4
#define COMMAND_W       5
#define COMMAND_M       6
#define COMMAND_D       7

#define RESET()                                                 \
    do {                                                        \
        command = 0;                                            \
        state = 0;                                              \
    } while (0)

#define LCDCHAR(c)                                              \
    lcd1602_putchar(c);

#define LCDSTR(str)                                             \
    print_str(lcd1602_putchar, 0, 0, str)


unsigned char input;
unsigned char state;
unsigned char command;


unsigned char hex2uchar(unsigned char hex)
{
    unsigned char t;

    t = hex - '0';
    if (t > 9) {
        t -= 'a' - '0' - 10;
        if (t < 10 || t > 15) {
            return 0xFF;
        }
    }
    return t;
}

void parse_pin(void)
{
    static unsigned char        pin;
    static __bit                b;
    unsigned char               t;

    switch (state) {
    case 0:
        pin = input - '0';
        if (pin < 4) {
            state += 1;
            return;
        }
        break;
    case 1:
        t = input - '0';
        if (t < 8) {
            pin = MEMORY_PIN(pin, t);
            b = (command == COMMAND_S);
            state += 1;
            return;
        }
        break;
    case 2:
        if (input == '\n') {
            if (command == COMMAND_B) {
                UARTCHAR('0' + memory_pin_get(pin));
            } else {
                memory_pin_set(pin, b);
            }
            RESET();
            return;
        }
        break;
    }

    state = STATE_FAIL;
}

void parse_port(void)
{
    static unsigned char        port;
    static unsigned char        c;
    unsigned char               t;

    switch (state) {
    case 0:
        port = input - '0';
        if (port < 4) {
            if (command == COMMAND_P) {
                state += 3;
            } else {
                state += 1;
            }
            return;
        }
        break;
    case 1:
        t = hex2uchar(input);
        if (t != 0xFF) {
            c = t << 4;
            state += 1;
            return;
        }
        break;
    case 2:
        t = hex2uchar(input);
        if (t != 0xFF) {
            c += t;
            state += 1;
            return;
        }
        break;
    case 3:
        if (input == '\n') {
            if (command == COMMAND_P) {
                UARTHEX2(memory_port_get(port));
            } else {
                memory_port_set(port, c);
            }
            RESET();
            return;
        }
        break;
    }

    state = STATE_FAIL;
}

void parse_display(void)
{
    if (input == '\n') {
        RESET();
    } else {
        LCDCHAR(input);
    }
}

void parse_move_cursor(void)
{
    static unsigned char        pos;
    unsigned char               t;

    switch (state) {
    case 0:
        t = hex2uchar(input);
        if (t != 0xFF) {
            pos = t << 4;
            state += 1;
            return;
        }
        break;
    case 1:
        t = hex2uchar(input);
        if (t != 0xFF) {
            pos += t;
            state += 1;
            return;
        }
        break;
    case 2:
        if (input == '\n') {
            lcd1602_position(pos & 0x0F, pos >> 4);
            RESET();
            return;
        }
        break;
    }

    state = STATE_FAIL;
}

void parse_command(void)
{
    switch (input) {
    case '@':
        uart_putchar('@');
        break;
    case 'b':
        command = COMMAND_B;
        break;
    case 's':
        command = COMMAND_S;
        break;
    case 'c':
        command = COMMAND_C;
        break;
    case 'p':
        command = COMMAND_P;
        break;
    case 'w':
        command = COMMAND_W;
        break;
    case 'd':
        command = COMMAND_D;
        break;
    case 'm':
        command = COMMAND_M;
        break;
    case '\n':
        break;
    default:
        state = STATE_FAIL;
        break;
    }
}

void parse_input(void)
{
    if (input == ESCAPE) {
        RESET();
        return;
    }
    if (input == '\r') {
        input = '\n';
    }
    switch (command) {
    case COMMAND_WAIT:
        parse_command();
        break;
    case COMMAND_B:
    case COMMAND_S:
    case COMMAND_C:
        parse_pin();
        break;
    case COMMAND_P:
    case COMMAND_W:
        parse_port();
        break;
    case COMMAND_D:
        parse_display();
        break;
    case COMMAND_M:
        parse_move_cursor();
        break;
    }
    if (state == STATE_FAIL) {
        UARTCHAR(ESCAPE);
        UARTCHAR('?');          /* For debugging using terminal */
        RESET();
    }
}

void main(void)
{
    lcd1602_init();
    LCDSTR("c51drv");

    while (uart_baudrate_auto());
    uart_init();
    UARTSTR("c51drv\n");

    while (1) {
        if (uart_rcready()) {
            input = uart_getchar();
            parse_input();
        }
    }
}
