/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __ADC_H
#define __ADC_H


#include "stc.h"


/* ADC_CONTR Register:
   POWER SPEED1 SPEED0 FLAG START CHS2 CHS1 CHS0 */

#define ADC_INIT(pins)                                          \
    do {                                                        \
        P1ASF = pins;                                           \
        AUXR1 &= ~0x04;                                         \
        ADC_CONTR = 0x80;                                       \
    } while (0)

#define ADC_START(pin, speed)                                   \
    do {                                                        \
        ADC_CONTR = (0x80 | (speed << 5) | 0x08 | pin);         \
    } while (0)

#define ADC_FLAG()                                              \
    (ADC_CONTR & 0x10)

#define ADC_CLEAR()                                             \
    do {                                                        \
        ADC_CONTR &= ~0x10;                                     \
    } while (0)

#define ADC_OFF()                                               \
    do {                                                        \
        ADC_CONTR = 0;                                          \
    } while (0)


#endif /* __ADC_H */
