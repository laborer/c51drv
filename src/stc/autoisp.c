/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "../common.h"
#include "stc.h"
#include "autoisp.h"


static unsigned char state = 0;


void autoisp_check(unsigned char c)
{       
    if ((unsigned char)AUTOISP_MAGIC[state] == c) {
        state += 1;
        if ((unsigned char)AUTOISP_MAGIC[state] == 0) {
            EA = 0;
            TXD = 0;
            delay_ms(AUTOISP_WAIT);
            /* Reboot to ISP section */
            /* IAPEN SWBS SWRST CMD_FAIL - WT2 WT1 WT0 */
            IAP_CONTR = 0x60;
        }
    } else {
        state = ((unsigned char)AUTOISP_MAGIC[0] == c) ? 1 : 0;
    }
}
