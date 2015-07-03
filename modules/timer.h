/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __TIMER_H
#define __TIMER_H


#include "common.h"


/* The increment of timer in t microseconds */
#define TIMER_CYCLES_US(t)                                      \
    ((uint16_t)((t) / 1000000.0 * FOSC / 12))

/* Timer flag indicating if timer overflows */
#define TIMER0_FLAG TF0

/* Start timer */
#define TIMER0_START()                                          \
    do {                                                        \
        TR0 = 1;                                                \
    } while (0)

/* Pause timer */
#define TIMER0_STOP()                                           \
    do {                                                        \
        TR0 = 0;                                                \
    } while (0)

/* Initialize Timer0 as a 16-bit timer */
#define TIMER0_INIT16()                                         \
    do {                                                        \
        /* Set Timer0 to be a 16-bit timer */                   \
        /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00              \
           -     -     -   -   0     0     0   1   */           \
        TMOD &= ~T0_MASK;                                       \
        TMOD |= T0_M0;                                          \
    } while (0)

/* Read Timer0 as a 16-bit timer */
#define TIMER0_GET16()                                          \
    timer0_get16()

/* Set Timer0 as a 16-bit timer */
#define TIMER0_SET16(t)                                         \
    do {                                                        \
        TL0 = (t);                                              \
        TH0 = (uint16_t)(t) >> 8;                               \
    } while (0)

/* Initialize Timer0 as an 8-bit auto-reload timer */
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

/* Read Timer0 as a 8-bit timer */
#define TIMER0_GET8()                                           \
    (TL0)

/* Set Timer0 as a 8-bit timer */
#define TIMER0_SET8(t)                                          \
    do {                                                        \
        TL0 = (t);                                              \
    } while (0)

/* Count down t microseconds */
#define TIMER0_DELAY_US(t)                                      \
    do {                                                        \
        TIMER0_STOP();                                          \
        TIMER0_INIT16();                                        \
        TIMER0_SET16(-CYCLES_US(t));                            \
        TIMER0_FLAG = 0;                                        \
        TIMER0_START();                                         \
        while (!TIMER0_FLAG);                                   \
    } while (0)
        

/* Timer flag indicating if timer overflows */
#define TIMER1_FLAG TF1

/* Start timer */
#define TIMER1_START()                                          \
    do {                                                        \
        TR1 = 1;                                                \
    } while (0)

/* Pause timer */
#define TIMER1_STOP()                                           \
    do {                                                        \
        TR1 = 0;                                                \
    } while (0)

/* Initialize Timer1 as a 16-bit timer */
#define TIMER1_INIT16()                                         \
    do {                                                        \
        /* Set Timer0 to be a 16-bit timer */                   \
        /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00              \
           0     0     0   1   -     -     -   -   */           \
        TMOD &= ~T1_MASK;                                       \
        TMOD |= T1_M0;                                          \
    } while (0)

/* Read Timer1 as a 16-bit timer.  Different than TIMER0_GET16(),
   TIMER1_GET16() can only get correct reading when the timer isn't
   running */
#define TIMER1_GET16()                                          \
    (TH1 * 256 + TL1)

/* Set Timer1 as a 16-bit timer */
#define TIMER1_SET16(t)                                         \
    do {                                                        \
        TL1 = (t);                                              \
        TH1 = (uint16_t)(t) >> 8;                               \
    } while (0)

/* Initialize Timer1 as an 8-bit auto-reload timer */
#define TIMER1_INIT8(t)                                         \
    do {                                                        \
        /* Set Timer0 to be an 8-bit auto-reload timer */       \
        /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00              \
           0     0     1   0   -     -     -   -   */           \
        TMOD &= ~T1_MASK;                                       \
        TMOD |= T1_M1;                                          \
        TR1 = 0;                                                \
        TF1 = 0;                                                \
        TH1 = (t);                                              \
    } while (0)


/* Define functions for Timer2 if the target MCU is 8052 */
#ifdef MICROCONTROLLER_8052

/* Timer flag indicating if timer overflows */
#define TIMER2_FLAG TF2

/* Start timer */
#define TIMER2_START()                                          \
    do {                                                        \
        TR2 = 1;                                                \
    } while (0)

/* Pause timer */
#define TIMER2_STOP()                                           \
    do {                                                        \
        TR2 = 0;                                                \
    } while (0)

/* Initialize Timer2 as a 16-bit timer */
#define TIMER2_INIT16()                                         \
    do {                                                        \
        /* Set Timer2 to be a 16-bit timer */                   \
        /* TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2# CP/RL2#           \
           0   0    0    0    0     0   0     0       */        \
        T2CON = 0;                                              \
    } while (0)

/* Read Timer2 as a 16-bit timer */
#define TIMER2_GET16()                                          \
    timer2_get16()

/* Set Timer2 as a 16-bit timer */
#define TIMER2_SET16(t)                                         \
    do {                                                        \
        RCAP2L = (t);                                           \
        RCAP2H = (uint16_t)(t) >> 8;                            \
    } while (0)

#endif /* MICROCONTROLLER_8052 */


#if defined SDCC || defined __SDCC
uint16_t timer0_get16(void) __naked;
#else
uint16_t timer0_get16(void);
#endif /* SDCC */


/* Declare TIMER0_MODE32_DISABLE to disable the quasi-32-bit timer so
   that user program can define their own Timer0 interrupt. */
#ifndef TIMER0_MODE32_DISABLE

/* Initialize Timer0 as a quasi-32-bit timer */
#define TIMER0_INIT32()                                         \
    do {                                                        \
        TIMER0_INIT16();                                        \
        /* Enable Timer0 interrupt */                           \
        ET0 = 1;                                                \
        /* Enable global interrupt */                           \
        EA = 1;                                                 \
    } while (0)

/* Read Timer2 as a quasi-32-bit timer */
#define TIMER0_GET32()                                          \
    timer0_get32()

/* Set Timer2 as a quasi-32-bit timer */
#define TIMER0_SET32(t)                                         \
    timer0_set32(t)


#if defined SDCC || defined __SDCC
uint32_t timer0_get32(void) __naked;
#else
uint32_t timer0_get32(void);
#endif /* SDCC */

void timer0_set32(uint32_t t);

#if defined SDCC || defined __SDCC
void timer0_interrupt(void) __interrupt TF0_VECTOR __using 1;
#endif /* SDCC */

#endif /* TIMER0_MODE32_DISABLE */


#endif /* __TIMER_H */
