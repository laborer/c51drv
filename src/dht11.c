/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "dht11.h"


#define DATA DHT11_DATA


char dht11_read(unsigned char __idata *out)
{
    unsigned char       i;
    unsigned char       j;
    unsigned char       k;
    unsigned char       p;
    unsigned char       c;

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
