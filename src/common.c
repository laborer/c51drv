/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"


/* Reverse the order of bits in a byte */
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

/* Convert a byte to packed BCD format */
unsigned char uchar2packedbcd(unsigned char x)
{
    return ((x / 10) << 4) | B;
}

/* Convert a byte to BCD format and store it in array buf */
void uchar2bcd(unsigned char x, unsigned char __idata *buf)
{
    x = x / 10;
    buf[2] = B;
    buf[0] = x / 10;
    buf[1] = B;
}

/* Convert an unsigned int to BCD format and store it in array buf.
   See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3731568
   See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3587651
   See also: http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 */
void uint2bcd(unsigned int x, unsigned char __idata *buf)
{
    unsigned char               i;
    unsigned char               j;
    unsigned char __idata       *p;

    i = x >> 10;        /* x == 1024i+r == 1000i+(24i+r-1024n) 
                           where n == 0 */
    if (i >= 42) {      /* if 24i > 1000 */
        i += 1;         /* let i++, n++; x == 1000i+(24i+r-1024n) */
    }                  
    j = i * 6 + x / 4;  /* j*4 == (24i+r-1024n)%1024 
                           == (24i+x-1024n)%1024 */
    if (CY) {           /* if 24i+r-1024n >= 1024 */
        j += 6;         /* let j*4 = (24i+r-1024n)%1024 + 24 */
        i += 1;         /* let i++, n++; x == 1000i+(24i+r-1024n) */
    }                   /* j*4 == (24i+r-1024n)%1024 */
    if (j >= 250) {     /* if j*4 >= 1000 */
        j += 6;         /* let j*4 = (24i+r-1024n)%1024 + 24 */
        i += 1;         /* let i++, n++; x == 1000i+(24i+r-1024n) */
    }                   /* j*4 == (24+r-1024n)%1024 and j*4 < 1000 */

    /* The reason a temporary pointer p is used here instead of d is
       that with SDCC, d is in data memory, so it needs to copy d to a
       register for each operation on the array in idata; while p is
       allocated to a register, so such problem doesn't exist. */
    p = buf;
    /* from MSD to LSD */
    *(p++) = i / 10;
    *(p++) = B;
    *(p++) = j / 25;
    *(p++) = (unsigned char)((unsigned char)(B * 4) + (x & 3)) / 10;
    *(p++) = B;
}


/* Convert an unsigned long to BCD format and store it in array buf */
void ulong2bcd(unsigned long x, unsigned char __idata *buf)
{
    unsigned char               i;
    unsigned char               j;
    unsigned char               k;
    unsigned char __idata       *p;

    /* Suppose the higher part of x is xh and the lower part of x is
       xl.  As x is an 32-bit integer, x == xh*65536+xl, which can be
       written in decimal arithmetic where xhn, xln to represent the
       n-th digit of xh's or xl's decimal representation, as follows,
                                    xh4   xh3   xh2   xh1   xh0
                                  *   6     5     5     3     6
                                 -------------------------------
                                   6xh4  6xh3  6xh2  6xh1  6xh0  
                             3xh4  3xh3  3xh2  3xh1  3xh0  
                       5xh4  5xh3  5xh2  5xh1  5xh0  
                 5xh4  5xh3  5xh2  5xh1  5xh0  
           6xh4  6xh3  6xh2  6xh1  6xh0  
          ------------------------------------------------------
              ?     ?     ?     ?     ?     ?     ?     ?     ?
            +                       xl4   xl3   xl2   xl1   xl0
          ------------------------------------------------------
                                                   x in decimal */
    uint2bcd(x, buf + 5);
    uint2bcd(x >> 16, buf);

    for (i = 5; i != 0; i--) {
        p = buf + i - 1;
        if (*p == 0) {
            continue;
        }
        j = *p * 5;       /* j = d[i - 1] * 5; */
        k = *p + j;       /* k = d[i - 1] * 6; */
        *p = 0;           /* d[i - 1] = 0 */
        *(++p) += k;      /* d[i + 0] += d[i - 1] * 6 */
        *(++p) += j;      /* d[i + 1] += d[i - 1] * 5 */
        *(++p) += j;      /* d[i + 2] += d[i - 1] * 5 */
        *(++p) += k / 2;  /* d[i + 3] += d[i - 1] * 3 */
        *(++p) += k;      /* d[i + 4] += d[i - 1] * 6 */
    }

    p = buf + 9;
    for (i = 9; i != 0; i--) {
        k = *p / 10;      /* k = d[i] / 10 */
        *p = B;           /* d[i] = d[i] % 10 */
        *(--p) += k;      /* d[i - 1] += k */
    }
}

/* Convert an unsigned int to hexadecimal and store it in array buf */
void uint2hex(unsigned int x, unsigned char __idata *buf)
{
    unsigned char __idata *p;

    p = buf;
    *(p++) = x >> 12;
    *(p++) = (x >> 8) & 0x0F;
    *(p++) = (unsigned char)x >> 4;
    *p = x & 0x0F;
}

/* Delay t milliseconds */
void delay_ms(unsigned int t)
{
    for (; t != 0; t--) {
        /* It needs about 6 extra machine cycles to do one loop. */
        DELAY_CYCLES(CYCLES_US(1000) - 6);
    }
}
