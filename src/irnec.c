/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"
#include "irnec.h"


#define BIT0            TIMER_CYCLES_US(562.5)
#define BIT1            (BIT0 * 3)
#define STATE2          ((unsigned int)(BIT0 * 15.5))
#define STATE3          (BIT0 * 8)
#define REPEAT          (BIT0 * 4)
#define TIME_ERROR      TIMER_CYCLES_US(100)


static unsigned char    state;
static unsigned int     time;
static unsigned int     data;
static unsigned char    buf;


void irnec_init(void)
{
    state = 1;
    buf = 0;

    TIMER0_INIT16();
    TIMER0_START();
}

char irnec_falling(void) 
{
    unsigned int t;
    unsigned int duration;

    t = timer0_get16();
    duration = t - time;
    time = t;

    if (state > 3) {
        if (duration > BIT0 + BIT1 + TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_LONGBIT;
        } else if (duration < BIT0 + BIT0 - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_SHORTBIT;
        }

        buf >>= 1;
        if (duration >= BIT0 + BIT1 - TIME_ERROR) {
            buf |= 0x80;
        }

        if ((state & 0xF) == 2) {
            if (state == 2 + 16 * 1) {
                data = buf << 8;
            } else if (state == 2 + 16 * 2) {
                if ((unsigned char)(~data >> 8) != buf) {
                    state = 2;
                    return IRNEC_ERR_INVERSE;
                }
            } else if (state == 2 + 16 * 3) {
                data |= buf;
            } else {  /* state == 2 + 16 * 4 */
                if ((unsigned char)(~data) != buf) {
                    state = 2;
                    return IRNEC_ERR_INVERSE;
                }
                state = 1;
                return 0;
            }
        }
        state += 2;
    } else if (state == 2) {
        if (duration > STATE2 + STATE3 + TIME_ERROR * 8) {
            state = 2;
            return IRNEC_ERR_LONGBIT;
        } else if (duration < STATE2 + REPEAT - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_SHORTBIT;
        } else if (duration < STATE2 + STATE3 - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_REPEAT;
        }
        state = 4;
    } else {
        state = 2;
    }

    return state;
}

unsigned int irnec_result(void)
{
    return data;
}
