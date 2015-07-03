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


void lcd1602_clear_display(void);
void lcd1602_return_home(void);
void lcd1602_entry_mode(__bit direction, __bit shift);
void lcd1602_switch(__bit display, __bit cursor, __bit blink);
void lcd1602_shift(__bit display, __bit direction);
void lcd1602_putchar(uint8_t c);
uint8_t lcd1602_getchar(void);
void lcd1602_position(uint8_t x, uint8_t y);
void lcd1602_write_cgram(uint8_t addr, uint8_t c);
uint8_t lcd1602_read_cgram(uint8_t addr);
void lcd1602_character(uint8_t idx, const uint8_t __code *p);
void lcd1602_init(void);


#endif //__LCD1602_H
