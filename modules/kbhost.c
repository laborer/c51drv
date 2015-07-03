/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "kbhost.h"


#define DATA    KBHOST_DATA
#define CLK     KBHOST_CLK

#if KBHOST_INTVEC == IE0_VECTOR
#define EXx     EX0
#define ITx     IT0
#define IEx     IE0
#elif KBHOST_INTVEC == IE1_VECTOR
#define EXx     EX1
#define ITx     IT1
#define IEx     IE0
#endif


#define CONDSET(cond, var, bit)                                 \
    do {                                                        \
        if (cond) {                                             \
            (var) |= (bit);                                     \
        } else {                                                \
            (var) &= ~(bit);                                    \
        }                                                       \
    } while(0)


static uint8_t  flags;
static uint8_t  newchar;
static uint8_t  modifiers;
static uint8_t  state;


/* Wait for CLK falling edge.  This function should only be used in
   interrupt routine */
static int8_t wait_clk_1(void) __using 1
{
    uint8_t i;
    
    for (i = 100; i != 0; i--) {
        if (CLK) {
            break;
        }
        DELAY_US(1);
    }
    if (i == 0) {
        return 2;
    }

    for (i = 100; i != 0; i--) {
        if (!CLK) {
            break;
        }
        DELAY_US(1);
    }

    return i == 0;
}

/* Wait for CLK falling edge.  This function should only be used in
   non-interrupt routine */
static int8_t wait_clk_0(void)
{
    uint8_t i;
    
    for (i = 100; i != 0; i--) {
        if (CLK) {
            break;
        }
        DELAY_US(1);
    }
    if (i == 0) {
        return 2;
    }

    for (i = 100; i != 0; i--) {
        if (!CLK) {
            break;
        }
        DELAY_US(1);
    }

    return i == 0;
}

/* Wait keyboard to be ready for receiving */
static int8_t wait_start(void)
{
    uint8_t i;
    
    for (i = 250; i != 0; i--) {
        if (!CLK) {
            break;
        }
        DELAY_US(10);
    }

    return i == 0;
}

/* Send a byte to keyboard and bypass its response */
static int8_t send_byte(uint8_t c)
{
    uint8_t     i;
    __bit       p;
    
    EXx = 0;
    CLK = 0;
    p = !parity(c);
    DELAY_US(60);
    DATA = 0;
    CLK = 1;

    if (wait_start()) {
        return 1;
    }

    for (i = 8; i != 0; i--) {
        DATA = c & 1;
        c >>= 1;
        if (wait_clk_0()) {
            return 1;
        }
    }

    DATA = p;
    if (wait_clk_0()) {
        return 1;
    }

    DATA = 1;
    if (wait_clk_0()) {
        return 1;
    }

    if (wait_clk_0() && wait_start()) {
        return 1;
    }

    for (i = 10; i != 0; i--) {
        if (wait_clk_0()) {
            return 1;
        }
    }
    return 0;
}

/* Translate keyboard scan code to ascii code.  This function also
   manages Num Lock, Caps Lock and Scroll Lock */
static void decode(void)
{
    const uint8_t __code graphmap[] = {
        0,    0,    0,    0,    0,    0,    0,    0,    
        0,    0,    0,    0,    0,    '\t', '`',  0,    
        0,    0,    0,    0,    0,    'q',  '1',  0,    
        0,    0,    'z',  's',  'a',  'w',  '2',  0,    
        0,    'c',  'x',  'd',  'e',  '4',  '3',  0,    
        0,    ' ',  'v',  'f',  't',  'r',  '5',  0,    
        0,    'n',  'b',  'h',  'g',  'y',  '6',  0,    
        0,    0,    'm',  'j',  'u',  '7',  '8',  0,    
        0,    ',',  'k',  'i',  'o',  '0',  '9',  0,    
        0,    '.',  '/',  'l',  ';',  'p',  '-',  0,    
        0,    0,    '\'', 0,    '[',  '=',  0,    0,    
        0,    0,    '\n', ']',  0,    '\\', 0,    0,    
        0,    0,    0,    0,    0,    0,    0,    0,    
        0,    '1',  0,    '4',  '7',  0,    0,    0,    
        '0',  '.',  '2',  '5',  '6',  '8',  0,    0,    
        0,    '+',  '3',  '-',  '*',  '9',  0,    0,
    };
    const uint8_t __code shiftmap[] = {
        0x45, ')',        
        0x16, '!',        
        0x1E, '@',        
        0x26, '#',        
        0x25, '$',        
        0x2E, '%',        
        0x36, '^',        
        0x3D, '&',        
        0x3E, '*',        
        0x46, '(', 
        0x0E, '~',        
        0x4E, '_',        
        0x55, '+',        
        0x5D, '|',        
        0x54, '{',        
        0x5B, '}',        
        0x4C, ':',        
        0x52, '"',        
        0x41, '<',        
        0x49, '>',        
        0x4A, '?',        
    };
    const uint8_t __code modifierlist[] = {
        0x12,        // KBHOST_LSHIFT
        0x59,        // KBHOST_RSHIFT
        0x14,        // KBHOST_LCTRL
        0x14 + 0x80, // KBHOST_RCTRL
        0x11,        // KBHOST_LALT
        0x11 + 0x80, // KBHOST_RALT
        0x1F + 0x80, // KBHOST_LGUI
        0x27 + 0x80, // KBHOST_RGUI
    };
    const uint8_t __code locklist[] = {
        0x7E,        // KBHOST_SCROLL
        0x77,        // KBHOST_NUM
        0x58,        // KBHOST_CAPS
    };

    uint8_t             i;
    uint8_t             c;
    uint8_t __code      *p;

    c = graphmap[newchar];
    if (newchar < 0x80 && c) {
        if (c >= 'a' && c <= 'z') {
            if (!(flags & KBHOST_SHIFT) != !(flags & KBHOST_CAPS)) {
                c += 'A' - 'a';
            }
        } else if (flags & KBHOST_SHIFT) {
            for (i = 0, p = shiftmap; i < sizeof(shiftmap) / 2; i++, p += 2) {
                if (newchar == *p) {
                    c = *(p + 1);
                    break;
                }
            }
        }
        newchar = c;
    } else if (newchar == 0x4A + 0x80) {
        // KP /
        newchar = '/';
    } else if (newchar == 0x5A + 0x80) {
        // KP EN
        newchar = '\n';
    } else {
        flags |= KBHOST_CONTROL;

        for (i = 1, p = modifierlist; i != 0; i <<= 1, p++) {
            if (newchar == *p) {
                CONDSET(!(flags & KBHOST_BREAK),
                        modifiers, i);
                CONDSET(modifiers & ~(KBHOST_LSHIFT | KBHOST_RSHIFT),
                        flags, KBHOST_MODIFIER);
                CONDSET(modifiers & (KBHOST_LSHIFT | KBHOST_RSHIFT),
                        flags, KBHOST_SHIFT);
                return;
            }
        }
        for (i = 1, p = locklist; i != 8; i <<= 1, p++) {
            if (newchar == *p) {
                if (!(flags & KBHOST_BREAK)) {
                    flags ^= i;
                }
                send_byte(0xED);
                send_byte(flags & 0x07);
                return;
            }
        }
    }
}


/* Interrupt routine for reading from keyboard */
void kbhost_interrupt(void) __interrupt KBHOST_INTVEC __using 1
{
    uint8_t     i;
    uint8_t     c;

    if (DATA) {
        return;
    }

    c = 0;
    for (i = 8; i != 0; i--) {
        if (wait_clk_1()) {
            return;
        }
        c >>= 1;
        if (DATA) {
            c |= 0x80;
        }
    }

    if (wait_clk_1()) {
        return;
    }

    // parity check

    if (wait_clk_1()) {
        return;
    }

    if (!DATA) {
        return;
    }

    EXx = 0;
    CLK = 0;

    newchar = c;
    flags |= KBHOST_HASNEW;
    
    return;
}

/* Initialization */
void kbhost_init(void)
{
    flags = KBHOST_NUM;
    modifiers = 0;
    state = 0;

    // DATA = 1
    DATA = 1;
    // CLK = 1;
    CLK = 1;
    // External interrupt triggered by a falling edge
    ITx = 1;
    // Disable external interrupt flag
    IEx = 0;
    // Enable external interrupt
    EXx = 1;
    // Enable global interrupt
    EA = 1;

    // Initialize LEDs
    send_byte(0xED);
    send_byte(flags & 0x07);
}

/* Stop keyboard from sending data */
void kbhost_stop(void)
{
    EXx = 0;
    CLK = 0;
}

/* Read a character sent by the keyboard */
int16_t kbhost_read(void)
{
    int16_t ret;

    if (!(flags & KBHOST_HASNEW)) {
        ret = 0;
    } else {
        flags &= ~KBHOST_HASNEW;

        if (state != 0) {
            state -= 1;
            ret = flags << 8;
        } else if (newchar == 0xF0) {
            // Break
            flags |= KBHOST_BREAK;
            ret = flags << 8;
        } else if (newchar == 0xE0) {
            // Extended
            flags |= KBHOST_CONTROL;
            ret = flags << 8;
        } else if (newchar == 0xE1) {
            // PAUSE
            state = 7;
            ret = (flags << 8) | 0xE1;
        } else {
            if (flags & KBHOST_CONTROL) {
                if (newchar == 0x12) {
                    // PRNT SCRN Make
                    state = 2;
                } else if (newchar == 0x7C) {
                    // PRNT SCRN Break
                    newchar = 0x12;
                    state = 3;
                }
                flags &= ~KBHOST_CONTROL;
                newchar |= 0x80;
            }
            decode();
            ret = (flags << 8) | newchar;
            flags &= ~(KBHOST_BREAK | KBHOST_CONTROL);
        }
    }

    /* Allow keyboard to send data */
    IEx = 0;
    EXx = 1;
    CLK = 1;
    return ret;
}

uint8_t kbhost_modifiers(void) 
{
    return modifiers;
}
