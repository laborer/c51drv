/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "tools.h"


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

/* See: http://www.hackersdelight.org/crc.pdf */
unsigned char crc8(unsigned char crc, unsigned char c)
{
    unsigned char i;

    crc ^= c;
    for (i = 8; i != 0; i--) {
        crc = (crc >> 1) ^ (-(crc & 1) & 0x8C);
    }

    return crc;
}
