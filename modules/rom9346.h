/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __ROM9346_H
#define __ROM9346_H


#include "common.h"


/* 93C46 16-bit mode */
#define ROM9346_ADDRLEN 6
typedef uint16_t rom9346_word_t;

/* 93C46 8-bit mode */
/* #define ROM9346_ADDR_LEN 7 */
/* typedef uint8_t rom9346_word_t; */

/* 93C56/93C66 16-bit mode */
/* #define ROM9346_ADDR_LEN 8 */
/* typedef uint16_t rom9346_word_t; */

/* 93C56/93C66 8-bit mode */
/* #define ROM9346_ADDR_LEN 9 */
/* typedef uint8_t rom9346_word_t; */

#define ROM9346_CS       P2_3


void rom9346_write_enable(void);
void rom9346_write_disable(void);
void rom9346_erase(uint16_t addr);
void rom9346_erase_all(void);
rom9346_word_t rom9346_read(uint16_t addr);
void rom9346_readstr(uint16_t addr,
                     rom9346_word_t __idata *p,
                     uint8_t n);
void rom9346_write(uint16_t addr, rom9346_word_t c);
void rom9346_write_all(rom9346_word_t c);


#endif /* __ROM9346_H */
