/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __LCD1602_H
#define __LCD1602_H


#define LCD1602_RS      P2_4
#define LCD1602_RW      P2_5
#define LCD1602_E       P2_6
#define LCD1602_DB      P0


void lcd1602_clear_display(void);
void lcd1602_return_home(void);
void lcd1602_entry_mode(__bit direction, __bit shift);
void lcd1602_switch(__bit display, __bit cursor, __bit blink);
void lcd1602_shift(__bit display, __bit direction);
void lcd1602_putchar(unsigned char c);
unsigned char lcd1602_getchar();
void lcd1602_position(unsigned char x, unsigned char y);
void lcd1602_write_cgram(unsigned char addr, unsigned char c);
unsigned char lcd1602_read_cgram(unsigned char addr);
void lcd1602_character(unsigned char idx, const unsigned char __code *p);
void lcd1602_init(void);


#endif //__LCD1602_H
