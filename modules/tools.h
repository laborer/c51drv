/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __TOOLS_H
#define __TOOLS_H


#include "common.h"


/* Return the number of elements in buf */
#define BUF_DATSIZE(buf)                                        \
    (sizeof((buf).dat) / sizeof((buf).dat[0]))
                                                        
/* Return the number of bits for the bitwise mask */    
#define BUF_BITS(buf)                                           \
    (((BUF_DATSIZE(buf) & 0x80) != 0)                           \
     + ((BUF_DATSIZE(buf) & 0xC0) != 0)                         \
     + ((BUF_DATSIZE(buf) & 0xE0) != 0)                         \
     + ((BUF_DATSIZE(buf) & 0xF0) != 0)                         \
     + ((BUF_DATSIZE(buf) & 0xF8) != 0)                         \
     + ((BUF_DATSIZE(buf) & 0xFC) != 0)                         \
     + ((BUF_DATSIZE(buf) & 0xFE) != 0))

/* Return the actual usable size of buf */
#define BUF_SIZE(buf)                                           \
    (1 << BUF_BITS(buf))

/* Return the bitwise mask for buf in/out index */
#define BUF_MASK(buf)                                           \
    (BUF_SIZE(buf) - 1)
    
/* Initialize buffer */
#define BUF_INIT(buf)                                           \
    do {                                                        \
        (buf).in = (buf).out = 0;                               \
    } while (0)

/* Pop an element from the head of the FIFO buffer */
#define BUF_GET(buf)                                            \
    ((buf).dat[(buf).out++ & BUF_MASK(buf)])

/* Append an element into the end of the FIFO buffer */
#define BUF_PUT(buf, c)                                         \
    do {                                                        \
        (buf).dat[(buf).in++ & BUF_MASK(buf)] = (c);            \
    } while (0)

/* Test if the buffer is empty */
#define BUF_EMPTY(buf)                                          \
    (!(((buf).in ^ (buf).out) & BUF_MASK(buf)))

/* Test if the buffer is full */
#define BUF_FULL(buf)                                           \
    ((sizeof((buf).in) == 1)                                    \
     ? (!((uint8_t)((buf).in - (buf).out + 1)                   \
          & BUF_MASK(buf)))                                     \
     : (!(((buf).in - (buf).out + 1) & BUF_MASK(buf))))

/* Return the number of elements in the buffer */
#define BUF_LEN(buf)                                            \
    ((sizeof((buf).in) == 1)                                    \
     ? ((uint8_t)((buf).in - (buf).out) & BUF_MASK(buf))        \
     : (((buf).in - (buf).out) & BUF_MASK(buf)))

/* Convert binary to byte.  E.g., BIN(11111111) returns 255 */
#define __BIN_FROM_HEX(h)                                       \
    (  ((h >> 21) & 0x80)                                       \
     | ((h >> 18) & 0x40)                                       \
     | ((h >> 15) & 0x20)                                       \
     | ((h >> 12) & 0x10)                                       \
     | ((h >>  9) & 0x08)                                       \
     | ((h >>  6) & 0x04)                                       \
     | ((h >>  3) & 0x02)                                       \
     | ((h >>  0) & 0x01))                       
#define BIN(b)                                                  \
    __BIN_FROM_HEX(0x##b##L)

/* Log base 2 of a byte x.  E.g. LOG2_8(16) returns 4 */
#define LOG2_8(x)                                               \
    (  (((x) >= 0x80) ? 1 : 0)                                  \
     + (((x) >= 0x40) ? 1 : 0)                                  \
     + (((x) >= 0x20) ? 1 : 0)                                  \
     + (((x) >= 0x10) ? 1 : 0)                                  \
     + (((x) >= 0x08) ? 1 : 0)                                  \
     + (((x) >= 0x04) ? 1 : 0)                                  \
     + (((x) >= 0x02) ? 1 : 0))

/* Minimum of a and b */
#define MIN(a, b)                                               \
    (((a) > (b)) ? (b) : (a))

/* Maximum of a and b */
#define MAX(a, b)                                               \
    (((a) > (b)) ? (a) : (b))

/* Convert a packed BCD number to a byte */
#define PACKEDBCD2UCHAR(x)                                      \
    ((uint8_t)(((uint8_t)(x) >> 4) * 10                         \
               + ((uint8_t)(x) & 0x0F)))

/* Reverse the order of a byte using a large lookup table.  Comment
   out this macro to save 256 bytes in code rom if it's not
   necessary */
#define REVERSE_FAST(c)                                         \
    reverse_lookup_table[c]

#ifdef REVERSE_FAST

#define REVERSE(c)                                              \
    REVERSE_FAST(c)

extern const uint8_t __code reverse_lookup_table[];

#else

#define REVERSE(c)                                              \
    reverse(c)

#endif


uint8_t crc8(uint8_t crc, uint8_t c);
uint8_t reverse(uint8_t c);
uint8_t uchar2packedbcd(uint8_t x);
void uchar2bcd(uint8_t x, uint8_t __idata *buf);
void uint2bcd(uint16_t x, uint8_t __idata *buf);
void ulong2bcd(uint32_t x, uint8_t __idata *buf);
void uint2hex(uint16_t x, uint8_t __idata *buf);
void delay_ms(uint16_t t);
__bit parity(uint8_t c);


#endif /* __TOOLS_H */
