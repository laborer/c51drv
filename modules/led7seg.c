/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"


/* Array of illuminated segments for each digit from 0 to F
       a
     +----+
   f | g  | b
     +----+
   e |    | c
     +----+
       d
  From MSB to LSB: - g f e d c b a */
const uint8_t __code led7seg_encode[16] = {
    BIN(00111111),  /* 0 */
    BIN(00000110),  /* 1 */
    BIN(01011011),  /* 2 */
    BIN(01001111),  /* 3 */
    BIN(01100110),  /* 4 */
    BIN(01101101),  /* 5 */
    BIN(01111101),  /* 6 */
    BIN(00000111),  /* 7 */
    BIN(01111111),  /* 8 */
    BIN(01101111),  /* 9 */
    BIN(01110111),  /* A */
    BIN(01111100),  /* b */
    BIN(00111001),  /* C */
    BIN(01011110),  /* d */
    BIN(01111001),  /* E */
    BIN(01110001)   /* F */
};
