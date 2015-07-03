/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"
#include "hcsr04.h"


#define TRIG    HCSR04_TRIG
#define ECHO    HCSR04_ECHO


uint16_t hcsr04_read(void)
{
    uint16_t t;

    TRIG = 1;
    DELAY_US(10);
    TRIG = 0;

    TIMER0_INIT16();
    TIMER0_START();

    while (!ECHO);
    t = timer0_get16();

    while (ECHO);
    t = timer0_get16() - t;

    return t;
}
