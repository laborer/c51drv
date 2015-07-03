/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __UART_H
#define __UART_H


#include "common.h"


/* Should SMOD bit be used in uart_baudrate() */
#define UART_SMOD       1
/* Baudrate after calling uart_baudrate() */
#define UART_BAUD       4800


#if defined SDCC || defined __SDCC
void uart_interrupt(void) __interrupt SI0_VECTOR __using 1;
#endif

int8_t uart_rcready(void);
int8_t uart_txready(void);
void uart_putchar(uint8_t c);
uint8_t uart_getchar(void);
void uart_baudrate(void);
int8_t uart_baudrate_auto(void);
void uart_init(void);


#endif /* __UART_H */
