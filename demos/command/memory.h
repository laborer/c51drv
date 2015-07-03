/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __MEMORY_H
#define __MEMORY_H


#include <common.h>


#define MEMORY_PIN(port, pin)                                   \
    (((port) << 3) + ((pin) & 0x7))


__bit memory_pin_get(unsigned char pin) __naked;
void memory_pin_set(unsigned char pin, __bit b) __naked;
unsigned char memory_port_get(unsigned char port) __naked;
void memory_port_set(unsigned char port, unsigned char c) __naked;
unsigned char memory_sfr_get(unsigned char sfr) __naked;
void memory_sfr_set(unsigned char sfr, unsigned char c) __naked;

    
#endif /* __MEMORY_H */
