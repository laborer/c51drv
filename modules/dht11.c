/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "dht11.h"


#define DATA DHT11_DATA


int8_t dht11_read(uint8_t __idata *out)
{
    uint8_t     i;
    uint8_t     j;
    uint8_t     k;
    uint8_t     p;
    uint8_t     c;

    if (DATA) {
        DATA = 0;
        delay_ms(18);
    }

    DATA = 1;

    for (k = 100; k != 0; k--) {
        if (!DATA) {
            break;
        }
        DELAY_US(1);
    }
    for (k = 100; k != 0; k--) {
        if (DATA) {
            break;
        }
        DELAY_US(1);
    }

    p = 0;
    c = 0;
    for (i = 5; i != 0; i--, out++) {
        for (j = 8; j != 0; j--) {
            for (k = 100; k != 0; k--) {
                if (!DATA) {
                    break;
                }
                DELAY_US(1);
            }
            for (k = 100; k != 0; k--) {
                if (DATA) {
                    break;
                }
                DELAY_US(1);
            }
            DELAY_US(28);
            c = (c << 1) | DATA;
        }
        if (i > 1) {
            *out = c;
            p += c;
        }
    }

    for (k = 100; k != 0; k--) {
        if (!DATA) {    
            break;
        }
        DELAY_US(1);
    }

    return (c == p) ? 0 : DHT11_ERR_CHECKSUM;
}
