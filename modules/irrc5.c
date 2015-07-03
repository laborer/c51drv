/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"
#include "irrc5.h"


#define BITHALF         TIMER_CYCLES_US(889)
#define TIME_ERROR      TIMER_CYCLES_US(200)


static uint8_t  state;
static uint16_t time;
static uint16_t data;


void irrc5_init(void)
{
    state = 1;
    data = 0;

    TIMER0_INIT16();
    TIMER0_START();
}

int8_t irrc5_falling(void)
{
    uint16_t t;

    t = time;
    time = timer0_get16();
    t = time - t;

    if (state & 1) {
        if (state == 1) {
            state = 2;
        } else {
            if (t > 3 * BITHALF + TIME_ERROR) {
                state = 2;
                return IRRC5_ERR_LONGBIT;
            } else if (t < 2 * BITHALF - TIME_ERROR) {
                state = 2;
                return IRRC5_ERR_SHORTBIT;
            }

            data <<= 1;
            if (t > 5 * BITHALF / 2) {
                /* 1|01|10| */
                state += 3;
                data += 1;
            } else {
                /* 1|01|01| */
                state += 2;
            }
        }
    } else {
        if (t > 4 * BITHALF + TIME_ERROR) {
            state = 2;
            return IRRC5_ERR_LONGBIT;
        } else if (t < 2 * BITHALF - TIME_ERROR) {
            state = 2;
            return IRRC5_ERR_SHORTBIT;
        } 
        
        if (t > 5 * BITHALF / 2) {
            data <<= 2;
            if (t > 7 * BITHALF / 2) {
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

int8_t irrc5_rising_last(void)
{
    uint16_t t;

    t = time;
    time = timer0_get16();

    /* This routine should only be called when the state is
       IRRC5_LASTBIT. */
    if (state != IRRC5_LASTBIT) {
        return IRRC5_ERR_STATE;
    }

    /* The state is reset to 1 rather than 2 because we are looking at
       a rising edge. */
    state = 1;

    t = time - t;
    if (t > 2 * BITHALF + TIME_ERROR) {
        return IRRC5_ERR_LONGBIT;
    } else if (t < BITHALF - TIME_ERROR) {
        return IRRC5_ERR_SHORTBIT;
    } 

    data <<= 1;
    if (t > 3 * BITHALF / 2) {
        data += 1;
    }
    return 0;
}

uint16_t irrc5_result(void)
{
    return data;
}
