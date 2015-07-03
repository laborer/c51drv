/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __HCSR04_H
#define __HCSR04_H


#include "common.h"


#define HCSR04_TRIG     P2_1
#define HCSR04_ECHO     P2_0


uint16_t hcsr04_read(void);


#endif /* __HCSR04_H */
