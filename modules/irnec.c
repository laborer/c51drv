/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "timer.h"
#include "irnec.h"


#define BIT0            TIMER_CYCLES_US(562.5)
#define BIT1            (BIT0 * 3)
#define STATE2          ((uint16_t)(BIT0 * 15.5))
#define STATE3          (BIT0 * 8)
#define REPEAT          (BIT0 * 4)
#define TIME_ERROR      TIMER_CYCLES_US(100)


static uint8_t  state;
static uint16_t time;
static uint16_t data;
static uint8_t  buf;


void irnec_init(void)
{
    state = 1;
    buf = 0;

    TIMER0_INIT16();
    TIMER0_START();
}

int8_t irnec_falling(void) 
{
    uint16_t t;

    t = time;
    time = timer0_get16();
    t = time - t;

    if (state > 3) {
        if (t > BIT0 + BIT1 + TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_LONGBIT;
        } else if (t < BIT0 + BIT0 - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_SHORTBIT;
        }

        buf >>= 1;
        if (t >= BIT0 + BIT1 - TIME_ERROR) {
            buf |= 0x80;
        }

        if ((state & 0xF) == 2) {
            if (state == 2 + 16 * 1) {
                data = buf << 8;
            } else if (state == 2 + 16 * 2) {
                if ((uint8_t)(~data >> 8) != buf) {
                    state = 2;
                    return IRNEC_ERR_INVERSE;
                }
            } else if (state == 2 + 16 * 3) {
                data |= buf;
            } else {  /* state == 2 + 16 * 4 */
                if ((uint8_t)(~data) != buf) {
                    state = 2;
                    return IRNEC_ERR_INVERSE;
                }
                state = 1;
                return 0;
            }
        }
        state += 2;
    } else if (state == 2) {
        if (t > STATE2 + STATE3 + TIME_ERROR * 8) {
            state = 2;
            return IRNEC_ERR_LONGBIT;
        } else if (t < STATE2 + REPEAT - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_SHORTBIT;
        } else if (t < STATE2 + STATE3 - TIME_ERROR) {
            state = 2;
            return IRNEC_ERR_REPEAT;
        }
        state = 4;
    } else {
        state = 2;
    }

    return state;
}

uint16_t irnec_result(void)
{
    return data;
}
