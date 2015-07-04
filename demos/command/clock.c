/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <timer.h>
#include <uart.h>
#include "clock.h"


#define CYCLES_1S ((uint32_t)TIMER_CYCLES_US(1000) * 1000)


#ifndef CLOCK_DATE_DISABLE

const __code uint8_t* __code clock_monthname[] = {
    "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
    "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

uint8_t         clock_year;
uint8_t         clock_month;
uint8_t         clock_day;

#endif /* CLOCK_DATE_DISALBE */

uint8_t         clock_hour;
uint8_t         clock_minute;
uint8_t         clock_second;

static uint32_t nextsec;


void clock_init(void)
{
    TIMER0_INIT32();
    TIMER0_SET32(0x11111111);
    TIMER0_START();
    ET0 = 0;
    nextsec = TIMER0_GET32() + CYCLES_1S;
    ET0 = 1;
}

uint8_t clock_update(void)
{
    int16_t     t;
    
    ET0 = 0;
    t = TIMER0_GET32() >> 16;
    ET0 = 1;

    /* if ((t - (short)(nextsec >> 16)) >= 0) { */
    t -= nextsec >> 16;
    if (t < 0) {
        return 0;
    }

    nextsec += CYCLES_1S;

    clock_second += 1;
    if (clock_second != 60) {
        return 1;
    }
    clock_second = 0;

    clock_minute += 1;
    if (clock_minute != 60) {
        return 2;
    }
    clock_minute = 0;

    clock_hour += 1;
    if (clock_hour != 24) {
        return 3;
    }
    clock_hour = 0;

#ifndef CLOCK_DATE_DISABLE
    {
        uint8_t     days;
    
        if (clock_month == 2) {
            days = (clock_year % 4) ? 3 : 2;
        } else {
            days = (uint8_t)(clock_month - 1) % 7 % 2;
        }
        days = 32 - days;

        clock_day += 1;
        if (clock_day != days) {
            return 4;
        }
        clock_day = 1;
        
        clock_month += 1;
        if (clock_month != 13) {
            return 5;
        }
        clock_month = 1;

        clock_year += 1;
        if (clock_year != 100) {
            return 6;
        }
        clock_year = 0;
    }
#endif /* CLOCK_DATE_DISABLE */

    return 1;
}
