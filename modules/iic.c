/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "iic.h"


#define SCL     IIC_SCL
#define SDA     IIC_SDA


/* Send byte c */
__bit iic_send(uint8_t c)
{
    uint8_t i;

    SCL = 0;

    for (i = 8; i != 0; i--) {
        SDA = c & 0x80;
        DELAY_US(2);
        SCL = 1;
        DELAY_US(1);
        c <<= 1;
        SCL = 0;
    }

    SDA = 1;
    DELAY_US(2);
    SCL = 1;
    DELAY_US(1);

    return SDA;
}

/* Receive a byte and acknowledge with bit ack */
uint8_t iic_recv(__bit ack)
{
    uint8_t     i;
    uint8_t     c;

    c = 0;

    SCL = 0;
    SDA = 1;
    DELAY_US(2);

    for (i = 8; i != 0; i--) {
        c <<= 1;
        SCL = 1;
        DELAY_US(1);
        c |= SDA;
        SCL = 0;
        DELAY_US(2);
    }

    SDA = ack;
    DELAY_US(1);
    SCL = 1;
    DELAY_US(1);

    return c;
}

/* Send iic start condition */
void iic_start(void)
{
    SCL = 0;
    DELAY_US(2);

    SDA = 1;
    SCL = 1;
    DELAY_US(2);
    SDA = 0;
    DELAY_US(2);
}

/* Send iic stop condition */
void iic_stop(void)
{
    SCL = 0;
    DELAY_US(2);
    
    SDA = 0;
    SCL = 1;
    DELAY_US(2);
    SDA = 1;
    DELAY_US(2);
}

/* Reset all iic slaves */
void iic_reset(void)
{
    uint8_t i;

    SCL = 0;
    SDA = 1;
    DELAY_US(2);
        
    for (i = 9; i != 0; i--) {
        SCL = 1;
        DELAY_US(2);
        SCL = 0;
        DELAY_US(2);
    }

    iic_start();
}
