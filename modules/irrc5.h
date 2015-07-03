/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __IRRC5_H
#define __IRRC5_H


#include "common.h"


#define IRRC5_IN                P3_2
#define IRRC5_OUT               P3_3

#define IRRC5_ERR_LONGBIT       -1
#define IRRC5_ERR_SHORTBIT      -2
#define IRRC5_ERR_STATE         -3

#define IRRC5_LASTBIT           (2 * 13)


void irrc5_init(void);
int8_t irrc5_falling(void);
int8_t irrc5_rising_last(void);
uint16_t irrc5_result(void);


#endif /* __IRRC5_H */
