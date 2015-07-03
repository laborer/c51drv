/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <common.h>


void main(void) {
    uint16_t i;

    P0 = 0;
    while (1) {
        P0 ^= 0xFF;
        for (i = 1000; i != 0; i--) {
            DELAY_US(1000);
        }
    }
}
