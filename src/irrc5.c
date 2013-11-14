/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"
#include "irrc5.h"


#define BITHALF         CYCLES_US(889)
#define TIME_ERROR      CYCLES_US(200)


static unsigned char    state;
static unsigned int     time;
static unsigned int     data;


void irrc5_init(void)
{
    state = 1;
    data = 0;

    TIMER0_INIT16();
    TIMER0_START();
}

char irrc5_falling(void)
{
    unsigned int t;
    unsigned int duration;

    t = timer0_get16();
    duration = t - time;
    time = t;

    if (state & 1) {
        if (state == 1) {
            state = 2;
        } else {
            if (duration > 3 * BITHALF + TIME_ERROR) {
                state = 2;
                return IRRC5_ERR_LONGBIT;
            } else if (duration < 2 * BITHALF - TIME_ERROR) {
                state = 2;
                return IRRC5_ERR_SHORTBIT;
            }

            data <<= 1;
            if (duration > 5 * BITHALF / 2) {
                /* 1|01|10| */
                state += 3;
                data += 1;
            } else {
                /* 1|01|01| */
                state += 2;
            }
        }
    } else {
        if (duration > 4 * BITHALF + TIME_ERROR) {
            state = 2;
            return IRRC5_ERR_LONGBIT;
        } else if (duration < 2 * BITHALF - TIME_ERROR) {
            state = 2;
            return IRRC5_ERR_SHORTBIT;
        } 
        
        if (duration > 5 * BITHALF / 2) {
            data <<= 2;
            if (duration > 7 * BITHALF / 2) {
                /* |10|01|10| */
                state += 4;
                data += 1;
            } else {
                /* |10|01|01| */
                state += 3;
            }
        } else {
            /* |10|10| */
            state += 2;
            data = (data << 1) + 1;
        }
    }

    if (state > 2 * 13) {
        state = 1;
        return 0;
    }
    
    return state;
}

char irrc5_rising_last(void)
{
    unsigned int t;
    unsigned int duration;

    t = timer0_get16();
    duration = t - time;
    time = t;

    /* This routine should only be called when the state is
       IRRC5_LASTBIT. */
    if (state != IRRC5_LASTBIT) {
        return IRRC5_ERR_STATE;
    }

    /* The state is reset to 1 rather than 2 because we are looking at
       a rising edge. */
    state = 1;

    if (duration > 2 * BITHALF + TIME_ERROR) {
        return IRRC5_ERR_LONGBIT;
    } else if (duration < BITHALF - TIME_ERROR) {
        return IRRC5_ERR_SHORTBIT;
    } 

    data <<= 1;
    if (duration > 3 * BITHALF / 2) {
        data += 1;
    }
    return 0;
}

unsigned int irrc5_result(void)
{
    return data;
}
