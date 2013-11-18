/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __ROM9346_H
#define __ROM9346_H


/* 93C46 16-bit mode */
#define ROM9346_ADDR_LEN 6
typedef unsigned int rom9346_word_t;

/* 93C46 8-bit mode */
/* #define ROM9346_ADDR_LEN 7 */
/* typedef unsigned char rom9346_word_t; */

/* 93C56/93C66 16-bit mode */
/* #define ROM9346_ADDR_LEN 8 */
/* typedef unsigned int rom9346_word_t; */

/* 93C56/93C66 8-bit mode */
/* #define ROM9346_ADDR_LEN 9 */
/* typedef unsigned char rom9346_word_t; */

#define ROM9346_CS       P2_3
#define ROM9346_CLK      P2_2
#define ROM9346_DI       P2_1
#define ROM9346_DO       P2_0


void rom9346_ewen();
void rom9346_ewds();
void rom9346_erase(unsigned int addr);
void rom9346_eral();
rom9346_word_t rom9346_read(unsigned int addr);
void rom9346_readstr(unsigned int addr, 
                     rom9346_word_t __idata *p,
                     unsigned char n);
void rom9346_write(unsigned int addr, rom9346_word_t c);
void rom9346_wral(rom9346_word_t c);


#endif /* __ROM9346_H */