/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __KBHOST_H
#define __KBHOST_H


#define KBHOST_DATA             P1_4
#define KBHOST_CLK              P3_2
#define KBHOST_INTVEC           IE0_VECTOR

#define KBHOST_ERR_NOACK        0x80
#define KBHOST_ERR_PARITY       0xA0

#define KBHOST_SCROLL           0x01
#define KBHOST_NUM              0x02
#define KBHOST_CAPS             0x04
#define KBHOST_SHIFT            0x08
#define KBHOST_MODIFIER         0x10
#define KBHOST_HASNEW           0x20
#define KBHOST_CONTROL          0x40
#define KBHOST_BREAK            0x80

#define KBHOST_LSHIFT           0x01
#define KBHOST_RSHIFT           0x02
#define KBHOST_LCTRL            0x04
#define KBHOST_RCTRL            0x08
#define KBHOST_LALT             0x10
#define KBHOST_RALT             0x20
#define KBHOST_LGUI             0x40
#define KBHOST_RGUI             0x80


#if defined SDCC || defined __SDCC
void kbhost_interrupt(void) __interrupt KBHOST_INTVEC __using 1;
#endif

void kbhost_init(void);
void kbhost_stop(void);
int16_t kbhost_read(void);
uint8_t kbhost_modifiers(void);


#endif /* __KBHOST_H */
