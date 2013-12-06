/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"


#if defined SDCC || defined __SDCC

/* Read Timer0 as a 16-bit timer.  It's safe to call it even the timer
   is still running.  For SDCC, this routine is implemented using
   inline assembly due to the inefficiency of the code SDCC (v3.2)
   generated */
unsigned int timer0_get16() __naked
{
    __asm
      loop1$:
        mov     a, _TH0
        mov     dpl, _TL0
        cjne    a, _TH0, loop1$
        mov     dph, a
        ret
    __endasm;
}

#else /* Other compiler */

/* Read Timer0 as a 16-bit timer.  It's safe to call it even the timer
   is still running */
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

/* Only 8052 or better MCUs have Timer2 */
#ifdef MICROCONTROLLER_8052

/* Read Timer2 as a 16-bit timer */
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

#if defined SDCC || defined __SDCC

static unsigned int __data t0_h32;

/* Read Timer0 as a quasi-32-bit timer.  It's safe to call it even the
   timer is still running.  For SDCC, this routine is implemented
   using inline assembly due to the inefficiency of the code SDCC
   (v3.2) generated */
uunsigned long timer0_get32() __naked
{
    __asm
      loop2$:
        mov     a, _TH0
        mov     dpl, _TL0
        mov     b, _t0_h32
        mov     dph, (_t0_h32 + 1)
        jb      _TF0, tfbegin$
      tfend$:
        cjne    a, _TH0, loop2$
        xch     a, dph
        ret

      tfbegin$:
        xch     a, b
        inc     a
        jnz     nocarry$
        inc     dph
      nocarry$:
        xch     a, b
        sjmp    tfend$
    __endasm;
}

#else /* Other compiler */

/* The higher 16-bit of Timer0 in quasi-32-bit mode */
static unsigned int t0_h32;

/* Read Timer0 as a quasi-16-bit timer.  It's safe to call it even the
   timer is still running */
unsigned long timer0_get32(void)
{
    unsigned int        t_h32;
    unsigned char       th, tl;
    
    do {
        th = TH0;
        tl = TL0;
        t_h32 = t0_h32;
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
           for interrupt routines. */
        if (TF0 == 1) {
            t_h32 += 1;
        }
    } while (th != TH0);

    return (long)t_h32 << 16 | ((int)th << 8) | tl;
}

#endif /* Other compiler */

/* Set Timer0 as a quasi-32-bit timer */
void timer0_set32(unsigned long t)
{
    TIMER0_SET16(t);
    t0_h32 = t >> 16;
}

#ifdef TIMER0_CALLBACK
extern void TIMER0_CALLBACK(void);
#endif /* TIMER0_CALLBACK */

/* Timer0 interrupt service routine for updating the higher 16-bit in
   quasi-16-bit mode */
void timer0_interrupt(void) __interrupt TF0_VECTOR __using 1
{
    TF0 = 0;
    t0_h32 += 1;
#ifdef TIMER0_CALLBACK
    TIMER0_CALLBACK();
#endif /* TIMER0_CALLBACK */
}
