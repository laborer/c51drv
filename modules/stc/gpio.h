/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __GPIO_H
#define __GPIO_H


#include "stc.h"


#define GPIO_BI         0
#define GPIO_OUT        1
#define GPIO_IN         2
#define GPIO_OD         3


#define GPIO_PINMODE(port, pin, mode)                           \
    do {                                                        \
        if ((mode) & 1) {                                       \
            P##port##M0 |= 1 << (pin);                          \
        } else {                                                \
            P##port##M0 &= ~(1 << (pin));                       \
        }                                                       \
        if ((mode) & 2) {                                       \
            P##port##M1 |= 1 << (pin);                          \
        } else {                                                \
            P##port##M1 &= ~(1 << (pin));                       \
        }                                                       \
    } while (0)

#define GPIO_PORTMODE(port, mode)                               \
    do {                                                        \
        P##port##M0 = ((mode) & 1) ? 0xFF : 0;                  \
        P##port##M1 = ((mode) & 2) ? 0xFF : 0;                  \
    } while (0)


#endif /* __GPIO_H */
