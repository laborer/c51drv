/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"


/* See: http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=3731568
   See: http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=3587651
   See also: http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 */
void hex2bcd(unsigned int x, char __idata *d)
{
    unsigned char i, j;

    i = x >> 10;
    if (i > 41)	{
        i += 1;
    }
    j = i * 6 + (unsigned char)(x >> 2);
    if (CY) {
        i += 1;
        j += 6;
    }
    if (j > 249) {
        i += 1;
        j += 6;
    }

    /* from MSD to LSD */
    d[0] = i / 10;
    d[1] = B;
    d[2] = j / 25;
    d[3] = (unsigned char)((B * 4) | (x & 0x3)) / 10;
    d[4] = B;
}

void delay_ms(unsigned int i)
{
    do {
        /* It needs about 6 extra machine cycles to do one loop. */
        DELAY_CYCLES(CYCLES_US(1000) - 6);
    } while (--i);
}
