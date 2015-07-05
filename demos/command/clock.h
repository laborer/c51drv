/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __CLOCK_H
#define __CLOCK_H


#include <timer.h>


#ifndef CLOCK_DATE_DISABLE

/* Calculate the number of days in the given month and year.  The
   result is valid for the 21st century. */
#define CLOCK_DAYSINMONTH(year, month)                          \
    ((uint8_t)(((month) == 2)                                   \
               ? (((year) % 4) ? 28 : 29)                       \
               : (31 - (uint8_t)((month) - 1) % 7 % 2)))

/* Check if the day of month overflows */
#define CLOCK_DAYOVERFLOW(year, month, day)                     \
    (day > 28 && day > CLOCK_DAYSINMONTH(year, month))


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
