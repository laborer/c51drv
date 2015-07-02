/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <common.h>
#include <uart.h>
#include <print.h>
#include <lcd1602.h>
#include <stc/stc.h>
#include "memory.h"


#define ESCAPE          0x1B
#define STATE_CMD_MASK  0xF0
#define STATE_CMD_B     0x10
#define STATE_CMD_S     0x20
#define STATE_CMD_C     0x30
#define STATE_CMD_P     0x40
#define STATE_CMD_W     0x50
#define STATE_CMD_M     0x60
#define STATE_CMD_D     0x70
#define STATE_FAIL      (0xFF & ~STATE_CMD_MASK)

#define LCDCHAR(c)                                              \
    lcd1602_putchar(c);

#define LCDSTR(str)                                             \
    print_str(lcd1602_putchar, 0, 0, str)


unsigned char input;
unsigned char state;


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

    switch (state & ~STATE_CMD_MASK) {
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
            b = ((state & STATE_CMD_MASK) == STATE_CMD_S);
            state += 1;
            return;
        }
        break;
    case 2:
        if (input == '\n') {
            if ((state & STATE_CMD_MASK) == STATE_CMD_B) {
                UARTCHAR('0' + memory_pin_get(pin));
            } else {
                memory_pin_set(pin, b);
            }
            state = 0;
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

    switch (state & ~STATE_CMD_MASK) {
    case 0:
        port = input - '0';
        if (port < 4) {
            if ((state & STATE_CMD_MASK) == STATE_CMD_P) {
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
            if ((state & STATE_CMD_MASK) == STATE_CMD_P) {
                UARTHEX2(memory_port_get(port));
            } else {
                memory_port_set(port, c);
            }
            state = 0;
            return;
        }
        break;
    }

    state = STATE_FAIL;
}

void parse_display(void)
{
    if (input == '\n') {
        state =0;
    } else {
        LCDCHAR(input);
    }
}

void parse_move_cursor(void)
{
    static unsigned char        pos;
    unsigned char               t;

    switch (state & ~STATE_CMD_MASK) {
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
            state = 0;
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
        state = STATE_CMD_B;
        break;
    case 's':
        state = STATE_CMD_S;
        break;
    case 'c':
        state = STATE_CMD_C;
        break;
    case 'p':
        state = STATE_CMD_P;
        break;
    case 'w':
        state = STATE_CMD_W;
        break;
    case 'd':
        state = STATE_CMD_D;
        break;
    case 'm':
        state = STATE_CMD_M;
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
        state = 0;
        return;
    }
    if (input == '\r') {
        input = '\n';
    }
    switch (state & STATE_CMD_MASK) {
    case 0:
        parse_command();
        break;
    case STATE_CMD_B:
    case STATE_CMD_S:
    case STATE_CMD_C:
        parse_pin();
        break;
    case STATE_CMD_P:
    case STATE_CMD_W:
        parse_port();
        break;
    case STATE_CMD_D:
        parse_display();
        break;
    case STATE_CMD_M:
        parse_move_cursor();
        break;
    }
    if (state == STATE_FAIL) {
        UARTCHAR(ESCAPE);
        UARTCHAR('?');          /* For debugging using terminal */
        state = 0;
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
