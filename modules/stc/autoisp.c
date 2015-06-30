/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "../common.h"
#include "../timer.h"
#include "stc.h"
#include "autoisp.h"


static unsigned char state = 0;


void autoisp_check(unsigned char c) __using 1
{
    unsigned int i;

    if ((unsigned char)AUTOISP_MAGIC[state] == c) {
        state += 1;
        if ((unsigned char)AUTOISP_MAGIC[state] == 0) {
            EA = 0;
            /* We are going to reboot.  Timer is free to use. */
            for (i = AUTOISP_WAIT; i != 0; i--) {
                TIMER0_DELAY_US(1000);
            }
            STC_REBOOT_ISP();
        }
    } else {
        state = ((unsigned char)AUTOISP_MAGIC[0] == c) ? 1 : 0;
    }
}
