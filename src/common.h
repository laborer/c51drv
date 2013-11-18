/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __COMMON_H
#define __COMMON_H


#ifndef FOSC
#define FOSC            12000000L
#endif /* FOSC */
#ifndef TICKS
#define TICKS           12
#endif /* TICKS */


#if defined SDCC || defined __SDCC /* SDCC */

#include <mcs51reg.h>

#define nop() __asm nop __endasm

#elif defined __C51__ || defined __CX51__ /* Keil C51 */

#include <regx51.h>

#define SI0_VECTOR      SIO_VECTOR
#define IDL             IDL_
#define SMOD            SMOD_
#define T1_M0           T1_M0_
#define T1_M1           T1_M1_
#define T1_MASK         T1_MASK_

#define __code          code
#define __data          data
#define __xdata         xdata
#define __bdata         bdata
#define __pdata         pdata
#define __idata         idata
#define __bit           bit
#define __reentrant     reentrant
#define __interrupt     interrupt
#define __using         using

void _nop_(void);

#define nop() _nop_()

#define __DELAY_MOV     2
#define __DELAY_TYPE    __data

#else /* Other compiler, finger crossed... */

#include <reg51.h>

#define nop() asm("NOP")

#endif /* Other compiler */


#define CYCLES_US(t)                                            \
    ((unsigned int)((t) / 1000000.0 * FOSC / TICKS))

#ifndef __DELAY_TYPE
#define __DELAY_TYPE
#endif /* __DELAY_TYPE */
#ifndef __DELAY_MOV
#define __DELAY_MOV     1
#endif /* __DELAY_MOV */
#define __DELAY_DJNZ    2
#define __DELAY_LOOP0   0x7E
#define __DELAY_INNER                                           \
    (__DELAY_MOV + __DELAY_DJNZ * (__DELAY_LOOP0 + 1))
#define __DELAY_LOOP1(n)                                        \
    (((n) - __DELAY_MOV) / __DELAY_INNER)
#define __DELAY_LOOP2(n)                                        \
    ((__DELAY_LOOP1(n) > 1)                                     \
     ? ((((n) - __DELAY_MOV) % __DELAY_INNER)                   \
        + __DELAY_INNER - __DELAY_MOV)                          \
     : ((n) - __DELAY_MOV))
#define DELAY_CYCLES(n)                                         \
    do {                                                        \
        __DELAY_TYPE unsigned char i;                           \
        __DELAY_TYPE unsigned char j;                           \
        if ((n) < 9) {                                          \
            if ((n) > 0) nop();                                 \
            if ((n) > 1) nop();                                 \
            if ((n) > 2) nop();                                 \
            if ((n) > 3) nop();                                 \
            if ((n) > 4) nop();                                 \
            if ((n) > 5) nop();                                 \
            if ((n) > 6) nop();                                 \
            if ((n) > 7) nop();                                 \
            break;                                              \
        }                                                       \
        if (__DELAY_LOOP1(n) > 1) {                             \
            for (i = __DELAY_LOOP1(n) - 1; i != 0; i--) {       \
                for (j = __DELAY_LOOP0; j != 0; j--);           \
            }                                                   \
        }                                                       \
        for (i = __DELAY_LOOP2(n) / __DELAY_DJNZ; i != 0; i--); \
        if (__DELAY_LOOP2(n) % __DELAY_DJNZ) nop();             \
    } while (0)
#define DELAY_US(t)                                             \
    DELAY_CYCLES(CYCLES_US(t))


unsigned char reverse(unsigned char c);
void uint2bcd(unsigned int x, unsigned char __idata *d);
void ulong2bcd(unsigned long x, unsigned char __idata *d);
void delay_ms(unsigned int i);


#endif //__COMMON_H
