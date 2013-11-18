/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __UART_H
#define __UART_H


#define UART_BAUD 4800

#if defined SDCC || defined __SDCC
void uart_interrupt(void) __interrupt SI0_VECTOR __using 1;
#endif


char uart_rcempty(void);
int uart_rcget(void);
char uart_txput(unsigned char c);

void uart_putchar(unsigned char c);
unsigned char uart_getchar(void);

void uart_baudrate(void);
char uart_baudrate_auto(void);
void uart_init(void);

void uart_putstr(const unsigned char __code *s);
void uart_putuint(unsigned int i);
void uart_putint(int i);


#endif /* __UART_H */
