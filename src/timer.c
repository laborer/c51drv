/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"


#if defined SDCC || defined __SDCC

unsigned int timer0_get16() __naked
{
    __asm
      00001$:
        mov     a, _TH0
        mov     dpl, _TL0
        cjne    a, _TH0, 00001$
        mov     dph, a
        ret
    __endasm;
}

#else /* Other compiler */

unsigned int timer0_get16()
{
    unsigned char th, tl;
    
    do {
        th = TH0;
        tl = TL0;
    } while (th != TH0);

    return ((int)th << 8) | tl;
}

#endif /* Other compiler */

#ifdef MICROCONTROLLER_8052

unsigned int timer2_get16()
{
    unsigned char th, tl;
    
    do {
        th = TH2;
        tl = TL2;
    } while (th != TH2);

    return ((int)th << 8) | tl;
}

#endif /* MICROCONTROLLER_8052 */

#ifndef NO_TIMER0_32

#if defined SDCC || defined __SDCC

static unsigned int __data t0_h32;

unsigned long timer0_get32() __naked
{
    __asm
      loop$:
        mov     a, _TH0
        mov     dpl, _TL0
        jbc     _TF0, tfbegin$
      tfend$:
        mov     b, _t0_h32
        mov     dph, (_t0_h32 + 1)
        cjne    a, _TH0, loop$
        xch     a, dph
        ret

      tfbegin$:
        xch     a, _t0_h32
        inc     a
        jnz     nocarry$
        inc     (_t0_h32 + 1)
      nocarry$:
        xch     a, _t0_h32
        sjmp    tfend$
    __endasm;
}

#else /* Other compiler */

static unsigned int t0_h32;

unsigned long timer0_get32(void)
{
    unsigned int        t_h32;
    unsigned char       th, tl;
    
    do {
        th = TH0;
        tl = TL0;
        /* The following part is to make sure that the higher 16-bit
           of the 32-bit timer increases even if timer0_get32(...) is
           called within an interrupt service routine.  Since this
           32-bit timer uses timer interrupt to maintain the higher
           16-bit part, if the control flow is already in an interrupt
           routine, the timer interrupt may not be invoked until the
           current interrupt routine exits.  In such case, the higher
           16-bit will not be correctly updated within the current
           interrupt routine, and timer0_get32(...) will return
           incorrect value.  Of course, if timer interrupt has higher
           priority than any other interrupt routines using
           timer0_get32(...), this problem will not occur, however, it
           might not always be feasible to have multiple priorities
           for interrupt routines.  To ensure the correctness of the
           aforementioned idea, testing TF0==1 and setting TF0=0 must
           be done atomically, as otherwise, it is possible to
           incorrectly increase to_h32 twice by the following
           statements and the timer interrupt routine under some
           improbable condition.  Therefore, it is recommended to use
           jbc instruction, which tests and clears a bit in one step.
           Normally, a decent compiler would use jbc.  */
        if (TF0 == 1) {
            TF0 = 0;
            t0_h32 += 1;
        }
        t_h32 = t0_h32;
    } while (th != TH0);

    return (long)t_h32 << 16 | ((int)th << 8) | tl;
}

#endif /* Other compiler */

void timer0_set32(unsigned long t)
{
    TIMER0_SET16(t);
    t0_h32 = t >> 16;
}

void timer0_interrupt32(void) __interrupt TF0_VECTOR __using 1
{
    TF0 = 0;
    t0_h32 += 1;
}

#endif /* NO_TIMER0_32 */