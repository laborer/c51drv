/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"


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
