/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __ROM2402_H
#define __ROM2402_H


#include "common.h"


#define ROM2402_PAGEBITS 3


uint8_t rom2402_read(uint8_t dev, uint8_t addr);
void rom2402_readstr(uint8_t dev,
                     uint8_t addr,
                     uint8_t __idata *p,
                     uint8_t n);
void rom2402_write(uint8_t dev, uint8_t addr, uint8_t c);
void rom2402_writestr(uint8_t dev,
                      uint8_t addr,
                      const uint8_t __idata *p,
                      uint8_t n);


#endif /* __ROM2402_H */
