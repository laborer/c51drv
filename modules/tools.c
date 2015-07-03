/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"


#ifdef REVERSE_FAST
const uint8_t __code reverse_lookup_table[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
#endif /* REVERSE_FAST */


/* CRC-8.
   See: http://www.hackersdelight.org/crc.pdf */
uint8_t crc8(uint8_t crc, uint8_t c)
{
    uint8_t i;

    crc ^= c;
    for (i = 8; i != 0; i--) {
        crc = (crc >> 1) ^ (-(crc & 1) & 0x8C);
    }

    return crc;
}

/* Reverse the order of a byte */
uint8_t reverse(uint8_t c)
{
    const uint8_t __code lookup[16] = {
        0x0, 0x8, 0x4, 0xC,
        0x2, 0xA, 0x6, 0xE,
        0x1, 0x9, 0x5, 0xD,
        0x3, 0xB, 0x7, 0xF
    };
    return (lookup[c & 0x0F] << 4) | lookup[c >> 4];
}

/* Convert a byte to packed BCD format */
uint8_t uchar2packedbcd(uint8_t x)
{
    return ((x / 10) << 4) | B;
}

/* Convert a byte to BCD format and store it in array buf */
void uchar2bcd(uint8_t x, uint8_t __idata *buf)
{
    x = x / 10;
    buf[2] = B;
    buf[0] = x / 10;
    buf[1] = B;
}

/* Convert an uint16_t to BCD format and store it in array buf.
   See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3731568
   See: http://www.amobbs.com/bbs/bbs_content.jsp?bbs_sn=3587651
   See also: http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 */
void uint2bcd(uint16_t x, uint8_t __idata *buf)
{
    uint8_t             i;
    uint8_t             j;
    uint8_t __idata     *p;

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
    *(p++) = (uint8_t)((uint8_t)(B * 4) + (x & 3)) / 10;
    *(p++) = B;
}


/* Convert an unsigned long (uint32_t) to BCD format and store it in
   array buf */
void ulong2bcd(uint32_t x, uint8_t __idata *buf)
{
    uint8_t             i;
    uint8_t             j;
    uint8_t             k;
    uint8_t __idata     *p;

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

/* Convert an uint16_t to hexadecimal and store it in array buf */
void uint2hex(uint16_t x, uint8_t __idata *buf)
{
    uint8_t __idata *p;

    p = buf;
    *(p++) = x >> 12;
    *(p++) = (x >> 8) & 0x0F;
    *(p++) = (uint8_t)x >> 4;
    *p = x & 0x0F;
}

/* Delay t milliseconds */
void delay_ms(uint16_t t)
{
    for (; t != 0; t--) {
        /* It needs about 6 extra machine cycles to do one loop. */
        DELAY_CYCLES(CYCLES_US(1000) - 6);
    }
}

/* Get the parity bit of c */
__bit parity(uint8_t c)
{
    ACC = c;
    return P;
}
