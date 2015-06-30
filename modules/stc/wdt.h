/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __WDT_H
#define __WDT_H


#include "../common.h"


#define WDT_KICK_IDLE_MS(idle, t)                               \
    (WDT_CONTR =                                                \
     0x20 /* Enable WDT */                                      \
     | 0x10 /* Clear WDT */                                     \
     | ((idle) ? 0x08 : 0) /* WDT in IDLE mode */               \
     | LOG2_8((float)FOSC * (t) / 1000 / 32768 / 12)            \
     )

#define WDT_KICK_MS(t)                                          \
    WDT_KICK_IDLE_MS(1, t)

#define WDT_KICK()                                              \
    (WDT_CONTR |= 0x10)

#define WDT_OVERFLOW_FLAG()                                     \
    (WDT_CONTR & 0x80)

#define WDT_OVERFLOW_CLEAR()                                    \
    do {                                                        \
        WDT_CONTR &= ~0x80;                                     \
    } while (0)
        

#endif /* __WDT_H */
