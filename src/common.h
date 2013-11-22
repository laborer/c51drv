/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __COMMON_H
#define __COMMON_H


#include "tools.h"


#ifndef FOSC
#define FOSC            12000000L
#endif /* FOSC */
#ifndef TICKS
#define TICKS           12
#endif /* TICKS */

#if TICKS == 6
#warning If you are using 6T mode for STC89 series MCUs,        \
    double FOSC rather than change TICKS from 12 to 6
#endif

/* R: Register
   N: Number
   D: Direct address 
   E: Relative address */
#ifndef CYCLES_MOV_R_N
#define CYCLES_MOV_R_N  1
#endif /* CYCLES_MOV_R_N */
#ifndef CYCLES_MOV_D_N
#define CYCLES_MOV_D_N  2
#endif /* CYCLES_MOV_D_N */
#ifndef CYCLES_DJNZ_R_E
#define CYCLES_DJNZ_R_E 2
#endif /* CYCLES_DJNZ_R_E */
#ifndef CYCLES_DJNZ_D_E
#define CYCLES_DJNZ_D_E 2
#endif /* CYCLES_DJNZ_D_E */


#if defined SDCC || defined __SDCC /* SDCC */

#include <mcs51reg.h>

#define nop() __asm nop __endasm

#define __DELAY_MOV     CYCLES_MOV_R_N
#define __DELAY_DJNZ    CYCLES_DJNZ_R_E
#define __DELAY_TYPE

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

#define __DELAY_MOV     CYCLES_MOV_D_N
#define __DELAY_DJNZ    CYCLES_DJNZ_D_E
#define __DELAY_TYPE    __data

#else /* Other compiler, finger crossed... */

#include <reg51.h>

#define nop() asm("NOP")

#define __DELAY_MOV     CYCLES_MOV_R_N
#define __DELAY_DJNZ    CYCLES_DJNZ_R_E
#define __DELAY_TYPE

#endif /* Other compiler */


#define CYCLES_US(t)                                            \
    ((unsigned int)((t) / 1000000.0 * FOSC / TICKS))

#define __DELAY_LOOP0   0x7E
#define __DELAY_INNER                                           \
    (__DELAY_MOV + __DELAY_DJNZ * (__DELAY_LOOP0 + 1))
#define __DELAY_CUTOFF(n)                                       \
    (((n) > 0) ? ((n) + __DELAY_MOV) : 0)
#define __DELAY_LOOP1(n)                                        \
    (((int)(n) - __DELAY_MOV) / __DELAY_INNER - 1)
#define __DELAY_N1(n)                                           \
    ((n) - __DELAY_CUTOFF(__DELAY_LOOP1(n) * __DELAY_INNER))
#define __DELAY_LOOP2(n)                                        \
    ((__DELAY_N1(n) <= 8) \
     ? 0 : ((__DELAY_N1(n) - __DELAY_MOV) / __DELAY_DJNZ))
#define __DELAY_N2(n)                                           \
    (__DELAY_N1(n)                                              \
     - __DELAY_CUTOFF(__DELAY_LOOP2(n) * __DELAY_DJNZ))
#define DELAY_CYCLES(n)                                         \
    do {                                                        \
        __DELAY_TYPE unsigned char i;                           \
        __DELAY_TYPE unsigned char j;                           \
        if (__DELAY_LOOP1(n) > 0) {                             \
            for (i = __DELAY_LOOP1(n); i != 0; i--) {           \
                for (j = __DELAY_LOOP0; j != 0; j--);           \
            }                                                   \
        }                                                       \
        if (__DELAY_LOOP2(n) > 0) {                             \
            for (i = __DELAY_LOOP2(n); i != 0; i--);            \
        }                                                       \
        if (__DELAY_N2(n) > 0) nop();                           \
        if (__DELAY_N2(n) > 1) nop();                           \
        if (__DELAY_N2(n) > 2) nop();                           \
        if (__DELAY_N2(n) > 3) nop();                           \
        if (__DELAY_N2(n) > 4) nop();                           \
        if (__DELAY_N2(n) > 5) nop();                           \
        if (__DELAY_N2(n) > 6) nop();                           \
        if (__DELAY_N2(n) > 7) nop();                           \
    } while (0)
#define DELAY_US(t)                                             \
    DELAY_CYCLES(CYCLES_US(t))


unsigned char reverse(unsigned char c);
unsigned char uchar2bcd(unsigned char x);
void uint2bcd(unsigned int x, unsigned char __idata *d);
void ulong2bcd(unsigned long x, unsigned char __idata *d);
void delay_ms(unsigned int i);


#endif /* __COMMON_H */
