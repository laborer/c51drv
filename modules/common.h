/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __COMMON_H
#define __COMMON_H


#include <stdint.h>


/* Frequency of the oscillator */
#ifndef FOSC
#define FOSC            12000000L
#endif /* FOSC */
/* Number of clock cycles per machine cycle */
#ifndef TICKS
#define TICKS           12
#endif /* TICKS */

/* STC89's 6T mode behaves more like doubling the frequency */
#if TICKS == 6
#warning If you are using 6T mode for STC89 series MCUs,        \
    double FOSC rather than change TICKS from 12 to 6
#endif

/* The number of machines cycles of the instructions.
   R: Register
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
#define PD              PD_
#define GF0             GF0_
#define GF1             GF1_
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


/* The number of machine cycles in a microsecond */
#define CYCLES_US(t)                                            \
    ((uint16_t)((t) / 1000000.0 * FOSC / TICKS))

/* The number of loops of the first inner loop */
#define __DELAY_LOOP0   0x7E
/* The number of machine cycles of the first inner loop */
#define __DELAY_INNER                                           \
    (__DELAY_MOV + __DELAY_DJNZ * (__DELAY_LOOP0 + 1))
/* Cannot to delay for negative machine cycles */
#define __DELAY_CUTOFF(n)                                       \
    (((n) > 0) ? ((n) + __DELAY_MOV) : 0)
/* The number of loops of the first outer loop */
#define __DELAY_LOOP1(n)                                        \
    (((int16_t)(n) - __DELAY_MOV) / __DELAY_INNER - 1)
/* The number of machine cycles left after the first loop */
#define __DELAY_N1(n)                                           \
    ((n) - __DELAY_CUTOFF(__DELAY_LOOP1(n) * __DELAY_INNER))
/* The number of loops of the second loop */
#define __DELAY_LOOP2(n)                                        \
    ((__DELAY_N1(n) <= 8) \
     ? 0 : ((__DELAY_N1(n) - __DELAY_MOV) / __DELAY_DJNZ))
/* The number of machine cycles left after the second loop */
#define __DELAY_N2(n)                                           \
    (__DELAY_N1(n)                                              \
     - __DELAY_CUTOFF(__DELAY_LOOP2(n) * __DELAY_DJNZ))
/* Delay for n machine cycles */
#define DELAY_CYCLES(n)                                         \
    do {                                                        \
        __DELAY_TYPE uint8_t i;                                 \
        __DELAY_TYPE uint8_t j;                                 \
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
/* Delay for t microseconds */
#define DELAY_US(t)                                             \
    DELAY_CYCLES(CYCLES_US(t))

/* Enter idle mode */
#define POWER_IDLE()                                            \
    do {                                                        \
        PCON |= IDL;                                            \
    } while (0)

/* Enter power down mode */
#define POWER_DOWN()                                            \
    do {                                                        \
        PCON |= PD;                                             \
    } while (0)


#endif /* __COMMON_H */
