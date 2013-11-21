/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __IIC_H
#define __IIC_H


#define IIC_SCL P1_1
#define IIC_SDA P1_2


__bit iic_send(unsigned char c);
unsigned char iic_recv(__bit ack);
void iic_start();
void iic_stop();
void iic_reset();


#endif /* __IIC_H */
