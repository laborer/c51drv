/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __AUTOISP_H
#define __AUTOISP_H


#include "../common.h"
#include "stc.h"


#ifndef AUTOISP_MAGIC
#define AUTOISP_MAGIC   "6af23Qtr"
#endif

#define AUTOISP_WAIT    1500

#ifdef AUTOISP
#define AUTOISP_CHECK(c)                                        \
    autoisp_check(c)
#else /* AUTOISP */
#define AUTOISP_CHECK(c)                                        \
    do {} while(0)
#endif /* AUTOISP */


void autoisp_check(uint8_t c) __using 1;


#endif /* __AUTOISP_H */
