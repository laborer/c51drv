/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"


unsigned char reverse(unsigned char c)
{
    const unsigned char __code lookup[16] = {
        0x0, 0x8, 0x4, 0xC,
        0x2, 0xA, 0x6, 0xE,
        0x1, 0x9, 0x5, 0xD,
        0x3, 0xB, 0x7, 0xF 
    };
    return (lookup[c & 0x0F] << 4) | lookup[c >> 4];
}

/* See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3731568
   See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3587651
   See also: http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 */
void uint2bcd(unsigned int x, unsigned char __idata *d)
{
    unsigned char               i;
    unsigned char               j;
    unsigned char __idata       *p;

    i = x >> 10;
    if (i >= 42) {
        i += 1;
    }
    j = i * 6 + x / 4;
    if (CY) {
        i += 1;
        j += 6;
    }
    if (j >= 250) {
        i += 1;
        j += 6;
    }

    /* from MSD to LSD */
    p = d;
    *(p++) = i / 10;
    *(p++) = B;
    *(p++) = j / 25;
    i = B * 4;
    i += x & 3;
    *(p++) = i / 10;
    *(p++) = B;
}

void ulong2bcd(unsigned long x, unsigned char __idata *d)
{
    unsigned char               i;
    unsigned char               j;
    unsigned char               k;
    unsigned char __idata       *p;

    uint2bcd(x, d + 5);
    uint2bcd(x >> 16, d);

    for (i = 5; i != 0; i--) {
        p = d + i - 1;
        j = *p * 5;       /* j = d[i - 1] * 5; */
        k = *p + j;       /* k = d[i - 1] * 6; */
        *p = 0;           /* d[i - 1] = 0 */
        *(++p) += k;      /* d[i + 0] += d[i - 1] * 6 */
        *(++p) += j;      /* d[i + 1] += d[i - 1] * 5 */
        *(++p) += j;      /* d[i + 2] += d[i - 1] * 5 */
        *(++p) += k / 2;  /* d[i + 3] += d[i - 1] * 3 */
        *(++p) += k;      /* d[i + 4] += d[i - 1] * 6 */
    }

    p = d + 9;
    for (i = 9; i != 0; i--) {
        k = *p / 10;      /* k = d[i] / 10 */
        *p = B;           /* d[i] = d[i] % 10 */
        *(--p) += k;      /* d[i - 1] += k */
    }
}

void delay_ms(unsigned int i)
{
    for (; i != 0; i--) {
        /* It needs about 6 extra machine cycles to do one loop. */
        DELAY_CYCLES(CYCLES_US(1000) - 6);
    }
}
