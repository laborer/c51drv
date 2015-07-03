/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __IRNEC_H
#define __IRNEC_H


#include "common.h"


#define IRNEC_ERR_LONGBIT       -1
#define IRNEC_ERR_SHORTBIT      -2
#define IRNEC_ERR_INVERSE       -3
#define IRNEC_ERR_REPEAT        -4


void irnec_init(void);
int8_t irnec_falling(void);
uint16_t irnec_result(void);


#endif /* __IRNEC_H */
