/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __CLOCK_H
#define __CLOCK_H


#include <timer.h>


/* This macro yields correct results for dates from Mar 1900 to Jan
   2100 */
#define CLOCK_DAYSINMONTH(year, month)                          \
    (32 - (((month) == 2)                                       \
           ? (((year) % 4) ? 3 : 2)                             \
           : (uint8_t)((month) - 1) % 7 % 2))


#ifndef CLOCK_DATE_DISABLE

extern const __code uint8_t* __code clock_monthname[];
extern const __code uint8_t* __code clock_dayofweekname[];

extern uint8_t  clock_year;
extern uint8_t  clock_month;
extern uint8_t  clock_day;

#endif /* CLOCK_DATE_DISALBE */

extern uint8_t  clock_hour;
extern uint8_t  clock_minute;
extern uint8_t  clock_second;


void clock_init(void);
uint8_t clock_update(void);

#ifndef CLOCK_DATE_DISABLE
uint8_t clock_dayofweek(void);
#endif /* CLOCK_DATE_DISALBE */


#endif /* __CLOCK_H */
