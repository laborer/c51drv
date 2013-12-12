/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __UART2_H
#define __UART2_H


#define UART2_SMOD      1
#define UART2_BAUD      4800
#define UART2_BRTX12    0


#if defined SDCC || defined __SDCC
void uart2_interrupt(void) __interrupt 8 __using 1;
#endif


char uart2_rcready(void);
char uart2_txready(void);
void uart2_putchar(unsigned char c);
unsigned char uart2_getchar(void);
void uart2_baudrate(void);
void uart2_init(void);


#endif /* __UART2_H */
