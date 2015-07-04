/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __CLOCK_H
#define __CLOCK_H


#include <timer.h>


#ifndef CLOCK_DATE_DISABLE

extern const __code uint8_t* __code clock_monthname[];

extern uint8_t  clock_year;
extern uint8_t  clock_month;
extern uint8_t  clock_day;

#endif /* CLOCK_DATE_DISALBE */

extern uint8_t  clock_hour;
extern uint8_t  clock_minute;
extern uint8_t  clock_second;


void clock_init(void);
uint8_t clock_update(void);


#endif /* __CLOCK_H */
