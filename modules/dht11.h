/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __DHT11_H
#define __DHT11_H


#include "common.h"


#define DHT11_DATA              P2_0

#define DHT11_ERR_CHECKSUM      1
#define DHT11_ERR_TIMEOUT       2


int8_t dht11_read(uint8_t __idata *out);


#endif /* __DHT11_H */
