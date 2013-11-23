/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __TIMER_H
#define __TIMER_H


#include "common.h"


#define TIMER_CYCLES_US(t)                                      \
    ((unsigned int)((t) / 1000000.0 * FOSC / 12))


#if defined SDCC || defined __SDCC

#pragma callee_saves timer0_get16,timer0_get32

#endif /* SDCC */


#define TIMER0_FLAG TF0

#define TIMER0_START()                                          \
    do {                                                        \
        TR0 = 1;                                                \
    } while (0)

#define TIMER0_STOP()                                           \
    do {                                                        \
        TR0 = 0;                                                \
    } while (0)

#define TIMER0_INIT16()                                         \
    do {                                                        \
        /* Set Timer0 to be a 16-bit timer */                   \
        /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00              \
           -     -     -   -   0     0     0   1   */           \
        TMOD &= ~T0_MASK;                                       \
        TMOD |= T0_M0;                                          \
        TR0 = 0;                                                \
        TF0 = 0;                                                \
    } while (0)

#define TIMER0_GET16()                                          \
    timer0_get16()

#define TIMER0_SET16(t)                                         \
    do {                                                        \
        TL0 = (t);                                              \
        TH0 = (unsigned int)(t) >> 8;                           \
    } while (0)

#define TIMER0_INIT8(t)                                         \
    do {                                                        \
        /* Set Timer0 to be an 8-bit auto-reload timer */       \
        /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00              \
           -     -     -   -   0     0     1   0   */           \
        TMOD &= ~T0_MASK;                                       \
        TMOD |= T0_M1;                                          \
        TR0 = 0;                                                \
        TF0 = 0;                                                \
        TH0 = (t);                                              \
    } while (0)

#define TIMER0_GET8()                                           \
    (TL0)

#define TIMER0_SET8(t)                                          \
    do {                                                        \
        TL0 = (t);                                              \
    } while (0)

#define TIMER0_COUNTDOWN_US(t)                                  \
    do {                                                        \
        TIMER0_INIT16();                                        \
        TIMER0_SET16(-CYCLES_US(t));                            \
        TIMER0_START();                                         \
    } while (0)
        
unsigned int timer0_get16();


#ifdef MICROCONTROLLER_8052

#define TIMER2_FLAG TF2

#define TIMER2_START()                                          \
    do {                                                        \
        TR2 = 1;                                                \
    } while (0)

#define TIMER2_STOP()                                           \
    do {                                                        \
        TR2 = 0;                                                \
    } while (0)

#define TIMER2_INIT16()                                         \
    do {                                                        \
        /* Set Timer2 to be a 16-bit auto-reload timer */       \
        /* TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2# CP/RL2#           \
           0   0    0    0    0     0   0     0       */        \
        T2CON = 0;                                              \
    } while (0)

#define TIMER2_GET16()                                          \
    timer2_get16()

#define TIMER2_SET16(t)                                         \
    do {                                                        \
        RCAP2L = (t);                                           \
        RCAP2H = (unsigned int)(t) >> 8;                        \
    } while (0)

#endif /* MICROCONTROLLER_8052 */


#define TIMER0_INIT32()                                         \
    do {                                                        \
        TIMER0_INIT16();                                        \
        /* Enable Timer0 interrupt */                           \
        ET0 = 1;                                                \
        /* Enable global interrupt */                           \
        EA = 1;                                                 \
    } while (0)

#define TIMER0_GET32()                                          \
    timer0_get32()

#define TIMER0_SET32(t)                                         \
    timer0_set32(t)

unsigned long timer0_get32(void);
void timer0_set32(unsigned long t);


#if defined SDCC || defined __SDCC
void timer0_interrupt(void) __interrupt TF0_VECTOR __using 1;
#endif /* SDCC */


#endif /* __TIMER_H */
