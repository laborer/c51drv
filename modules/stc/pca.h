/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __PCA_H
#define __PCA_H


#include "stc.h"


/* PCA Control Register: CCON
   CF CR - - - - CCF1 CCF0
   
   PCA Mode Register: CMOD
   CIDL - - - CPS2 CPS1 CPS0 ECF
   
   PCA Module n Mode Register: CCAPMn
   - ECOMn CAPPn CAPNn MATn TOGn PWMn ECCFn

   PCA Counter: CL / CH
   
   PCA Comparator: CCAPnL

   PCA PWM Mode Auxiliary Register n: PCA_PWMn
   - - - - - - EPCnH EPCnL */

#define PCA_FALLING     0x10
#define PCA_RISING      0x20

#define PCA_SYSCLK_12   0x00
#define PCA_SYSCLK_2    0x02
#define PCA_T0          0x04
#define PCA_ECI         0x06
#define PCA_SYSCLK      0x08
#define PCA_SYSCLK_4    0x0A
#define PCA_SYSCLK_6    0x0C
#define PCA_SYSCLK_8    0x0E

#define PCA0_FLAG       CCF0
#define PCA1_FLAG       CCF1
  
#define PCA_INIT(clock_source)                                  \
    do {                                                        \
        CCON = 0;                                               \
        CMOD &= ~0x0E;                                          \
        CMOD |= clock_source;                                   \
        CL = 0;                                                 \
        CH = 0;                                                 \
        CCAPM0 = 0;                                             \
        CCAPM1 = 0;                                             \
        CR = 1;                                                 \
    } while (0)

#define PCA0_INTR(enable)                                       \
    do {                                                        \
        CCAPM0 |= (enable) ? 1 : 0;                             \
    } while (0)

/* E.g. PCA0_CAPTURE(PCA_FALLING | PCA_RISING) */
#define PCA0_CAPTURE(edge)                                      \
    do {                                                        \
        CCAPM0 = (edge);                                        \
    } while (0)

#define PCA0_TIMER(t)                                           \
    do {                                                        \
        CCAP0L = (t) & 0xFF;                                    \
        CCAP0H = (t) >> 8;                                      \
        CCAPM0 = 0x48;                                          \
    } while (0)

#define PCA0_SQUAREOUT(t)                                       \
    do {                                                        \
        CCAP0L = (t) & 0xFF;                                    \
        CCAP0H = (t) >> 8;                                      \
        CCAPM0 = 0x4C;                                          \
    } while (0)

#define PCA0_PWMOUT(duty)                                       \
    do {                                                        \
        PCA_PWM0 = 0;                                           \
        CCAP0H = (duty);                                        \
        CCAP0L = (duty);                                        \
        CCAPM0 = 0x42;                                          \
    } while (0)

#define PCA0_OFF()                                              \
    do {                                                        \
        CCAPM0 = 0;                                             \
    } while (0)

#define PCA1_INTR(enable)                                       \
    do {                                                        \
        CCAPM1 |= (enable) ? 1 : 0;                             \
    } while (0)

#define PCA1_CAPTURE(edge)                                      \
    do {                                                        \
        CCAPM1 = (edge);                                        \
    } while (0)

#define PCA1_TIMER(t)                                           \
    do {                                                        \
        CCAP1L = (t) & 0xFF;                                    \
        CCAP1H = (t) >> 8;                                      \
        CCAPM1 = 0x48;                                          \
    } while (0)

#define PCA1_SQUAREOUT(t)                                       \
    do {                                                        \
        CCAP1L = (t) & 0xFF;                                    \
        CCAP1H = (t) >> 8;                                      \
        CCAPM1 = 0x4C;                                          \
    } while (0)

#define PCA1_PWMOUT(duty)                                       \
    do {                                                        \
        PCA_PWM1 = 0;                                           \
        CCAP1H = (duty);                                        \
        CCAP1L = (duty);                                        \
        CCAPM1 = 0x42;                                          \
    } while (0)

#define PCA1_OFF()                                              \
    do {                                                        \
        CCAPM1 = 0;                                             \
    } while (0)


#endif /* __PCA_H */
