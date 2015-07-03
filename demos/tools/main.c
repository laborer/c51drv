/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>


void main(void) {
    unsigned int                i;
    unsigned char __idata       p[5];
    unsigned char               mask;

    i = 42025;
    uint2bcd(i, p);

    if (p[0] == 4 && p[1] == 2 && p[2] == 0 && p[3] == 2 && p[4] == 5) {
        mask = 0x0F;
    } else {
        mask = 0xF0;
    }

    P0 = 0;
    while (1) {
        P0 ^= mask;
        delay_ms(1000);
    }

    while (1);
}
