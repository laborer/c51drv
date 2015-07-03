/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __IIC_H
#define __IIC_H


#include "common.h"


#define IIC_SCL P1_1
#define IIC_SDA P1_2


__bit iic_send(uint8_t c);
uint8_t iic_recv(__bit ack);
void iic_start(void);
void iic_stop(void);
void iic_reset(void);


#endif /* __IIC_H */
