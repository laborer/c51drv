/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <lcd1602.h>
#include "memory.h"
#include "clock.h"


#define ESCAPE          0x1B
#define COMMAND_FAIL    0xFF
#define NCOMMANDS       (sizeof(command_table) / sizeof(command_table[0]))
#define NBUFFER         6

#define RESET()                                                 \
    do {                                                        \
        command = 0;                                            \
        length = 0;                                             \
    } while (0)

#define LCD_PRINTF_C(c)                                         \
    lcd1602_putchar(c);

#define LCD_PRINTF_S(str)                                       \
    print_str(lcd1602_putchar, 0, 0, str)

#define LCD_PRINTF_02D(num)                                     \
    print_int(lcd1602_putchar, PRINT_ZERO, 2, num);


const __code struct {
    uint8_t     cmd;
    uint8_t     len;
    __bit       (*func)(void);
} command_table[] = {
    {'\n', 1, parse_command},   /* 0 */
    {'b', 3, parse_pin},        /* 1 */
    {'s', 3, parse_pin},        /* 2 */
    {'c', 3, parse_pin},        /* 3 */
    {'p', 2, parse_port},       /* 4 */
    {'w', 4, parse_port},       /* 5 */
    {'m', 3, parse_movecursor}, /* 6 */
    {'l', 1, parse_display},    /* 7 */
    {'d', 7, parse_date},       /* 8 */
    {'t', 7, parse_time},       /* 9 */
};

uint8_t input;
uint8_t length;
uint8_t command;
uint8_t buffer[NBUFFER];


static uint8_t decode_hex(uint8_t hex)
{
    hex -= '0';
    if (hex > 9) {
        hex -= 'a' - '0' - 10;
        if (hex < 10 || hex > 15) {
            return 0xFF;
        }
    }

    return hex;
}

static __bit decode_dec_buffer()
{
    uint8_t i;

    for (i = 0; i != 6; i++) {
        buffer[i] -= '0';
        if (buffer[i] > 9) {
            return 1;
        }
    }

    for (i = 0; i != 3; i++) {
        buffer[i] = buffer[i * 2] * 10 + buffer[i * 2 + 1];
    }

    return 0;
}

void display_clock(uint8_t update)
{
    uint8_t addr;

    if (update == 0) {
        return;
    }

    addr = lcd1602_getaddr();

    if (update > 3) {
        LCD1602_POSITION(0, 0);
        LCD_PRINTF_S(clock_dayofweekname[clock_dayofweek()]);
        LCD_PRINTF_C(' ');
        LCD_PRINTF_S(clock_monthname[clock_month - 1]);
        LCD_PRINTF_C(' ');
        LCD_PRINTF_02D(clock_day);
        LCD_PRINTF_S(" 20");
        LCD_PRINTF_02D(clock_year);
    }
    if (update > 1) {
        LCD1602_POSITION(0, 1);
        LCD_PRINTF_02D(clock_hour);
        LCD_PRINTF_C(':');
        LCD_PRINTF_02D(clock_minute);
        LCD_PRINTF_C(':');
    }
    LCD1602_POSITION(6, 1);
    LCD_PRINTF_02D(clock_second);

    LCD1602_SETADDR(addr);
}

__bit parse_pin(void)
{
    if (input != '\n') {
        return 1;
    }

    buffer[0] -= '0';
    buffer[1] -= '0';
    if (buffer[0] >= 4 || buffer[1] >= 8) {
        return 1;
    }

    buffer[0] = MEMORY_PIN(buffer[0], buffer[1]);
    if (command = 1) {          /* B */
        UARTCHAR('0' + memory_pin_get(buffer[0]));
    } else {                    /* S, C */
        memory_pin_set(buffer[0], command == 2);
    }

    RESET();
    return 0;
}

__bit parse_port(void)
{
    if (input != '\n') {
        return 1;
    }

    buffer[0] -= '0';
    if (buffer[0] >= 4) {
        return 1;
    }

    if (command == 4) {
        UARTHEX2(memory_port_get(buffer[0]));
    } else {
        buffer[1] = decode_hex(buffer[1]);
        buffer[2] = decode_hex(buffer[2]);
        if (buffer[1] == 0xFF || buffer[2] == 0xFF) {
            return 1;
        }
        memory_port_set(buffer[0], (buffer[1] << 4) | buffer[2]);
    }

    RESET();
    return 0;
}

__bit parse_display(void)
{
    if (input == '\n') {
        RESET();
    } else {
        LCD_PRINTF_C(input);
        length = 0;
    }

    return 0;
}

__bit parse_movecursor(void)
{
    if (input != '\n') {
        return 1;
    }

    buffer[0] = decode_hex(buffer[0]);
    buffer[1] = decode_hex(buffer[1]);
    if (buffer[0] == 0xFF || buffer[1] == 0xFF) {
        return 1;
    }

    LCD1602_POSITION(buffer[1], buffer[0]);

    RESET();
    return 0;
}

__bit parse_date(void)
{
    if (input != '\n') {
        return 1;
    }

    if (decode_dec_buffer()
        || buffer[1] == 0
        || buffer[1] > 12
        || buffer[2] == 0
        || CLOCK_DAYOVERFLOW(buffer[0], buffer[1], buffer[2])) {
        return 1;
    }

    clock_year = buffer[0];
    clock_month = buffer[1];
    clock_day = buffer[2];

    display_clock(6);

    RESET();
    return 0;
}

__bit parse_time(void)
{
    if (input != '\n') {
        return 1;
    }

    if (decode_dec_buffer()
        || buffer[0] >= 24
        || buffer[1] >= 60
        || buffer[2] >= 60) {
        return 1;
    }

    clock_hour = buffer[0];
    clock_minute = buffer[1];
    clock_second = buffer[2];

    clock_init();
    display_clock(3);

    RESET();
    return 0;
}

__bit parse_command(void)
{
    uint8_t i;

    for (i = 0; i != NCOMMANDS; i++) {
        if (input == command_table[i].cmd) {
            command = i;
            length = 0;
            return 0;
        }
    }

    if (input == '@') {
        UARTCHAR('@');
        length = 0;
        return 0;
    }

    return 1;
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
    if (command == COMMAND_FAIL) {
        goto fail;
    }

    length += 1;
    if (length == command_table[command].len) {
        if (command_table[command].func()) {
            command = COMMAND_FAIL;
            goto fail;
        }
    } else if (input == '\n') {
        goto fail;
    } else {
        buffer[length - 1] = input;
    }

    return;

 fail:
    if (input == '\n') {
        UARTCHAR(ESCAPE);
        UARTCHAR('?');          /* For debugging using terminal */
        RESET();
    }
}

void main(void)
{
    lcd1602_init();

    /* while (uart_baudrate_auto()); */
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");

    clock_year = 16;
    clock_month = 12;
    clock_day = 31;
    clock_hour = 23;
    clock_minute = 59;
    clock_second = 55;
    clock_init();
    display_clock(6);

    while (1) {
        if (uart_rcready()) {
            input = uart_getchar();
            parse_input();
        }

        display_clock(clock_update());
    }
}
