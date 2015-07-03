/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <common.h>
#include "memory.h"


__bit memory_pin_get(unsigned char pin) __naked
{
    ACC = pin;
    __asm
        /* Do we need to preserve PSW as ADD instruction changes both
           C and OV flags? */
        add     a, #0x100-4*8
        jnc     #pin_get_valid$
        ret
      pin_get_valid$:
        mov     a, dpl
        rl      a
        add     a, dpl
        mov     dptr, #pin_get_table$
        jmp     @a+dptr
      pin_get_table$:
        mov     c, 0x80
        ret
        mov     c, 0x81
        ret
        mov     c, 0x82
        ret
        mov     c, 0x83
        ret
        mov     c, 0x84
        ret
        mov     c, 0x85
        ret
        mov     c, 0x86
        ret
        mov     c, 0x87
        ret
        mov     c, 0x90
        ret
        mov     c, 0x91
        ret
        mov     c, 0x92
        ret
        mov     c, 0x93
        ret
        mov     c, 0x94
        ret
        mov     c, 0x95
        ret
        mov     c, 0x96
        ret
        mov     c, 0x97
        ret
        mov     c, 0xa0
        ret
        mov     c, 0xa1
        ret
        mov     c, 0xa2
        ret
        mov     c, 0xa3
        ret
        mov     c, 0xa4
        ret
        mov     c, 0xa5
        ret
        mov     c, 0xa6
        ret
        mov     c, 0xa7
        ret
        mov     c, 0xb0
        ret
        mov     c, 0xb1
        ret
        mov     c, 0xb2
        ret
        mov     c, 0xb3
        ret
        mov     c, 0xb4
        ret
        mov     c, 0xb5
        ret
        mov     c, 0xb6
        ret
        mov     c, 0xb7
        ret
    __endasm;
}

void memory_pin_set(unsigned char pin, __bit b) __naked
{
    ACC = pin;
    __asm
        add     a, #0x100-4*8
        jnc     #pin_set_valid$
        ret
      pin_set_valid$:
        mov     a, dpl
        rl      a
        add     a, dpl
        mov     dptr, #pin_set_table$
    __endasm;
    CY = b;
    __asm
        jmp     @a+dptr
      pin_set_table$:
        mov     0x80, c
        ret
        mov     0x81, c
        ret
        mov     0x82, c
        ret
        mov     0x83, c
        ret
        mov     0x84, c
        ret
        mov     0x85, c
        ret
        mov     0x86, c
        ret
        mov     0x87, c
        ret
        mov     0x90, c
        ret
        mov     0x91, c
        ret
        mov     0x92, c
        ret
        mov     0x93, c
        ret
        mov     0x94, c
        ret
        mov     0x95, c
        ret
        mov     0x96, c
        ret
        mov     0x97, c
        ret
        mov     0xa0, c
        ret
        mov     0xa1, c
        ret
        mov     0xa2, c
        ret
        mov     0xa3, c
        ret
        mov     0xa4, c
        ret
        mov     0xa5, c
        ret
        mov     0xa6, c
        ret
        mov     0xa7, c
        ret
        mov     0xb0, c
        ret
        mov     0xb1, c
        ret
        mov     0xb2, c
        ret
        mov     0xb3, c
        ret
        mov     0xb4, c
        ret
        mov     0xb5, c
        ret
        mov     0xb6, c
        ret
        mov     0xb7, c
        ret
    __endasm;
}

unsigned char memory_port_get(unsigned char port) __naked
{
    ACC = port;
    __asm
        add     a, #0x100-4
        jnc     #port_get_valid$
        ret
      port_get_valid$:
        mov     a, dpl
        rl      a
        rl      a
        mov     dptr, #port_get_table$
        jmp     @a+dptr
      port_get_table$:
        mov     dpl, 0x80
        ret
        mov     dpl, 0x90
        ret
        mov     dpl, 0xa0
        ret
        mov     dpl, 0xb0
        ret
    __endasm;
}

void memory_port_set(unsigned char port, unsigned char c) __naked
{
    ACC = port;
    B = c;
    __asm
        add     a, #0xff-0x03
        jnc     #port_set_valid$
        ret
      port_set_valid$:
        mov     a, dpl
        rl      a
        rl      a
        mov     dptr, #port_set_table$
        jmp     @a+dptr
      port_set_table$:
        mov     0x80, b
        ret
        mov     0x90, b
        ret
        mov     0xa0, b
        ret
        mov     0xb0, b
        ret
    __endasm;
}

unsigned char memory_sfr_get(unsigned char sfr) __naked
{
    ACC = sfr;
    __asm
        clr     c
        rlc     a
        jc      sfr_get_valid$
        ret
      sfr_get_valid$:
        mov     dptr, #sfr_get_table$
        clr     c
        rlc     a
        jnc     sfr_get_mov$
        inc     dph
      sfr_get_mov$:
        jmp     @a+dptr
      sfr_get_table$:
        mov     dpl, 0x80
        ret
        mov     dpl, 0x81
        ret
        mov     dpl, 0x82
        ret
        mov     dpl, 0x83
        ret
        mov     dpl, 0x84
        ret
        mov     dpl, 0x85
        ret
        mov     dpl, 0x86
        ret
        mov     dpl, 0x87
        ret
        mov     dpl, 0x88
        ret
        mov     dpl, 0x89
        ret
        mov     dpl, 0x8a
        ret
        mov     dpl, 0x8b
        ret
        mov     dpl, 0x8c
        ret
        mov     dpl, 0x8d
        ret
        mov     dpl, 0x8e
        ret
        mov     dpl, 0x8f
        ret
        mov     dpl, 0x90
        ret
        mov     dpl, 0x91
        ret
        mov     dpl, 0x92
        ret
        mov     dpl, 0x93
        ret
        mov     dpl, 0x94
        ret
        mov     dpl, 0x95
        ret
        mov     dpl, 0x96
        ret
        mov     dpl, 0x97
        ret
        mov     dpl, 0x98
        ret
        mov     dpl, 0x99
        ret
        mov     dpl, 0x9a
        ret
        mov     dpl, 0x9b
        ret
        mov     dpl, 0x9c
        ret
        mov     dpl, 0x9d
        ret
        mov     dpl, 0x9e
        ret
        mov     dpl, 0x9f
        ret
        mov     dpl, 0xa0
        ret
        mov     dpl, 0xa1
        ret
        mov     dpl, 0xa2
        ret
        mov     dpl, 0xa3
        ret
        mov     dpl, 0xa4
        ret
        mov     dpl, 0xa5
        ret
        mov     dpl, 0xa6
        ret
        mov     dpl, 0xa7
        ret
        mov     dpl, 0xa8
        ret
        mov     dpl, 0xa9
        ret
        mov     dpl, 0xaa
        ret
        mov     dpl, 0xab
        ret
        mov     dpl, 0xac
        ret
        mov     dpl, 0xad
        ret
        mov     dpl, 0xae
        ret
        mov     dpl, 0xaf
        ret
        mov     dpl, 0xb0
        ret
        mov     dpl, 0xb1
        ret
        mov     dpl, 0xb2
        ret
        mov     dpl, 0xb3
        ret
        mov     dpl, 0xb4
        ret
        mov     dpl, 0xb5
        ret
        mov     dpl, 0xb6
        ret
        mov     dpl, 0xb7
        ret
        mov     dpl, 0xb8
        ret
        mov     dpl, 0xb9
        ret
        mov     dpl, 0xba
        ret
        mov     dpl, 0xbb
        ret
        mov     dpl, 0xbc
        ret
        mov     dpl, 0xbd
        ret
        mov     dpl, 0xbe
        ret
        mov     dpl, 0xbf
        ret
        mov     dpl, 0xc0
        ret
        mov     dpl, 0xc1
        ret
        mov     dpl, 0xc2
        ret
        mov     dpl, 0xc3
        ret
        mov     dpl, 0xc4
        ret
        mov     dpl, 0xc5
        ret
        mov     dpl, 0xc6
        ret
        mov     dpl, 0xc7
        ret
        mov     dpl, 0xc8
        ret
        mov     dpl, 0xc9
        ret
        mov     dpl, 0xca
        ret
        mov     dpl, 0xcb
        ret
        mov     dpl, 0xcc
        ret
        mov     dpl, 0xcd
        ret
        mov     dpl, 0xce
        ret
        mov     dpl, 0xcf
        ret
        mov     dpl, 0xd0
        ret
        mov     dpl, 0xd1
        ret
        mov     dpl, 0xd2
        ret
        mov     dpl, 0xd3
        ret
        mov     dpl, 0xd4
        ret
        mov     dpl, 0xd5
        ret
        mov     dpl, 0xd6
        ret
        mov     dpl, 0xd7
        ret
        mov     dpl, 0xd8
        ret
        mov     dpl, 0xd9
        ret
        mov     dpl, 0xda
        ret
        mov     dpl, 0xdb
        ret
        mov     dpl, 0xdc
        ret
        mov     dpl, 0xdd
        ret
        mov     dpl, 0xde
        ret
        mov     dpl, 0xdf
        ret
        mov     dpl, 0xe0
        ret
        mov     dpl, 0xe1
        ret
        mov     dpl, 0xe2
        ret
        mov     dpl, 0xe3
        ret
        mov     dpl, 0xe4
        ret
        mov     dpl, 0xe5
        ret
        mov     dpl, 0xe6
        ret
        mov     dpl, 0xe7
        ret
        mov     dpl, 0xe8
        ret
        mov     dpl, 0xe9
        ret
        mov     dpl, 0xea
        ret
        mov     dpl, 0xeb
        ret
        mov     dpl, 0xec
        ret
        mov     dpl, 0xed
        ret
        mov     dpl, 0xee
        ret
        mov     dpl, 0xef
        ret
        mov     dpl, 0xf0
        ret
        mov     dpl, 0xf1
        ret
        mov     dpl, 0xf2
        ret
        mov     dpl, 0xf3
        ret
        mov     dpl, 0xf4
        ret
        mov     dpl, 0xf5
        ret
        mov     dpl, 0xf6
        ret
        mov     dpl, 0xf7
        ret
        mov     dpl, 0xf8
        ret
        mov     dpl, 0xf9
        ret
        mov     dpl, 0xfa
        ret
        mov     dpl, 0xfb
        ret
        mov     dpl, 0xfc
        ret
        mov     dpl, 0xfd
        ret
        mov     dpl, 0xfe
        ret
        mov     dpl, 0xff
        ret
    __endasm;
}

void memory_sfr_set(unsigned char sfr, unsigned char c) __naked
{
    ACC = sfr;
    B = c;
    __asm
        clr     c
        rlc     a
        jc      sfr_set_valid$
        ret
      sfr_set_valid$:
        mov     dptr, #sfr_set_table$
        clr     c
        rlc     a
        jnc     sfr_set_mov$
        inc     dph
      sfr_set_mov$:
        jmp     @a+dptr
      sfr_set_table$:
        mov     0x80, b
        ret
        mov     0x81, b
        ret
        mov     0x82, b
        ret
        mov     0x83, b
        ret
        mov     0x84, b
        ret
        mov     0x85, b
        ret
        mov     0x86, b
        ret
        mov     0x87, b
        ret
        mov     0x88, b
        ret
        mov     0x89, b
        ret
        mov     0x8a, b
        ret
        mov     0x8b, b
        ret
        mov     0x8c, b
        ret
        mov     0x8d, b
        ret
        mov     0x8e, b
        ret
        mov     0x8f, b
        ret
        mov     0x90, b
        ret
        mov     0x91, b
        ret
        mov     0x92, b
        ret
        mov     0x93, b
        ret
        mov     0x94, b
        ret
        mov     0x95, b
        ret
        mov     0x96, b
        ret
        mov     0x97, b
        ret
        mov     0x98, b
        ret
        mov     0x99, b
        ret
        mov     0x9a, b
        ret
        mov     0x9b, b
        ret
        mov     0x9c, b
        ret
        mov     0x9d, b
        ret
        mov     0x9e, b
        ret
        mov     0x9f, b
        ret
        mov     0xa0, b
        ret
        mov     0xa1, b
        ret
        mov     0xa2, b
        ret
        mov     0xa3, b
        ret
        mov     0xa4, b
        ret
        mov     0xa5, b
        ret
        mov     0xa6, b
        ret
        mov     0xa7, b
        ret
        mov     0xa8, b
        ret
        mov     0xa9, b
        ret
        mov     0xaa, b
        ret
        mov     0xab, b
        ret
        mov     0xac, b
        ret
        mov     0xad, b
        ret
        mov     0xae, b
        ret
        mov     0xaf, b
        ret
        mov     0xb0, b
        ret
        mov     0xb1, b
        ret
        mov     0xb2, b
        ret
        mov     0xb3, b
        ret
        mov     0xb4, b
        ret
        mov     0xb5, b
        ret
        mov     0xb6, b
        ret
        mov     0xb7, b
        ret
        mov     0xb8, b
        ret
        mov     0xb9, b
        ret
        mov     0xba, b
        ret
        mov     0xbb, b
        ret
        mov     0xbc, b
        ret
        mov     0xbd, b
        ret
        mov     0xbe, b
        ret
        mov     0xbf, b
        ret
        mov     0xc0, b
        ret
        mov     0xc1, b
        ret
        mov     0xc2, b
        ret
        mov     0xc3, b
        ret
        mov     0xc4, b
        ret
        mov     0xc5, b
        ret
        mov     0xc6, b
        ret
        mov     0xc7, b
        ret
        mov     0xc8, b
        ret
        mov     0xc9, b
        ret
        mov     0xca, b
        ret
        mov     0xcb, b
        ret
        mov     0xcc, b
        ret
        mov     0xcd, b
        ret
        mov     0xce, b
        ret
        mov     0xcf, b
        ret
        mov     0xd0, b
        ret
        mov     0xd1, b
        ret
        mov     0xd2, b
        ret
        mov     0xd3, b
        ret
        mov     0xd4, b
        ret
        mov     0xd5, b
        ret
        mov     0xd6, b
        ret
        mov     0xd7, b
        ret
        mov     0xd8, b
        ret
        mov     0xd9, b
        ret
        mov     0xda, b
        ret
        mov     0xdb, b
        ret
        mov     0xdc, b
        ret
        mov     0xdd, b
        ret
        mov     0xde, b
        ret
        mov     0xdf, b
        ret
        mov     0xe0, b
        ret
        mov     0xe1, b
        ret
        mov     0xe2, b
        ret
        mov     0xe3, b
        ret
        mov     0xe4, b
        ret
        mov     0xe5, b
        ret
        mov     0xe6, b
        ret
        mov     0xe7, b
        ret
        mov     0xe8, b
        ret
        mov     0xe9, b
        ret
        mov     0xea, b
        ret
        mov     0xeb, b
        ret
        mov     0xec, b
        ret
        mov     0xed, b
        ret
        mov     0xee, b
        ret
        mov     0xef, b
        ret
        mov     0xf0, b
        ret
        mov     0xf1, b
        ret
        mov     0xf2, b
        ret
        mov     0xf3, b
        ret
        mov     0xf4, b
        ret
        mov     0xf5, b
        ret
        mov     0xf6, b
        ret
        mov     0xf7, b
        ret
        mov     0xf8, b
        ret
        mov     0xf9, b
        ret
        mov     0xfa, b
        ret
        mov     0xfb, b
        ret
        mov     0xfc, b
        ret
        mov     0xfd, b
        ret
        mov     0xfe, b
        ret
        mov     0xff, b
        ret
    __endasm;
}
