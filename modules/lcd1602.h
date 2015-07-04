/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __LCD1602_H
#define __LCD1602_H


#include "common.h"


#define LCD1602_RS      P2_4
#define LCD1602_RW      P2_5
#define LCD1602_E       P2_6
#define LCD1602_DB      P0

#define LCD1602_CLEAR()                                         \
    lcd1602_cmd(0x01)

#define LCD1602_HOME()                                          \
    lcd1602_cmd(0x02)

#define LCD1602_ENTRY(direction, shift)                         \
    lcd1602_cmd(0x04 | ((__bit)direction << 1) | shift)

#define LCD1602_SWITCH(display, cursor, blink)                  \
    lcd1602_cmd(0x08                                            \
                | ((__bit)(display) << 2)                       \
                | ((__bit)(cursor) << 1)                        \
                | (__bit)(blink))

#define LCD1602_SHIFT(display, direction)                       \
    lcd1602_cmd(0x10                                            \
                | ((__bit)display << 3)                         \
                | ((__bit)direction << 2))

#define LCD1602_SETADDR(addr)                                   \
    lcd1602_cmd(0x80 | (addr))

#define LCD1602_POSITION(x, y)                                  \
    LCD1602_SETADDR(x | (y << 6))


uint8_t lcd1602_getaddr(void);
void lcd1602_cmd(uint8_t cmd);
uint8_t lcd1602_getchar();
void lcd1602_putchar(uint8_t c);
void lcd1602_character(uint8_t idx, const uint8_t __code *p);
void lcd1602_init(void);


#endif //__LCD1602_H
