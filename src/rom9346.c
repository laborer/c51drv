/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "rom9346.h"


#define CS              ROM9346_CS
#define CLK             ROM9346_CLK
#define DI              ROM9346_DI
#define DO              ROM9346_DO
#define ADDR_LEN        ROM9346_ADDR_LEN
#define word_t          rom9346_word_t


/* Send low n bits in c to the chip */
static void send_bits(unsigned char c, unsigned char n)
{
    c <<= 8 - n;
    for (; n != 0; n--) {
        CLK = 0;
        DELAY_US(1);
        DI = c & 0x80;
        CLK = 1;
        DELAY_US(1);
        c <<= 1;
    }
}

/* Send an instruction to the chip */
static void send_cmd(unsigned char opcode, unsigned int addr)
{
    CS = 0;
    DELAY_US(1);

    CS = 1;

    DO = 1;
    while (!DO);

    send_bits(4 | opcode, 3);
    if (ADDR_LEN > 8) {
        send_bits(addr >> 8, ADDR_LEN - 8);
        send_bits(addr, 8);
    } else {
        send_bits(addr, ADDR_LEN);
    }
}

/* Receive a word from the chip */
static word_t recv_word()
{
    word_t              c;
    unsigned char       i;

    c = 0;
    for (i = sizeof(c) * 8; i != 0; i--) {
        CLK = 0;
        DELAY_US(1);
        CLK = 1;
        DELAY_US(1);
        c <<= 1;
        c |= DO;
    }

    return c;
}

/* Enable erase and write. */
void rom9346_ewen()
{
    /* Clear CS first to reset the internal control logic */
    send_cmd(0, 3 << (ADDR_LEN - 2));
    CS = 0;
}

/* Disable erase and write. */
void rom9346_ewds()
{
    send_cmd(0, 0 << (ADDR_LEN - 2));
    CS = 0;
}

/* Erase one word. */
void rom9346_erase(unsigned int addr)
{
    send_cmd(3, addr);
    CS = 0;
}

/* Erase all. */
void rom9346_eral()
{
    send_cmd(0, 2 << (ADDR_LEN - 2));
    CS = 0;
}

/* Read one word from addr */
word_t rom9346_read(unsigned int addr)
{
    word_t c;

    send_cmd(2, addr);
    c = recv_word();
    CS = 0;

    return c;
}

/* Read n words from addr to string p */
void rom9346_readstr(unsigned int addr, word_t __idata *p, unsigned char n)
{
    send_cmd(2, addr);
    for (; n != 0; p++, n--) {
        *p = recv_word();
    }
    CS = 0;
}

/* Write one word c to addr */
void rom9346_write(unsigned int addr, word_t c)
{
    send_cmd(1, addr);
    if (sizeof(c) == 2) {
        send_bits(c >> 8, 8);
    }
    send_bits(c, 8);
    CS = 0;
}

/* Write c to all memory.  Note: this doesn't erase automatically. */
void rom9346_wral(word_t c)
{
    send_cmd(0, 1 << (ADDR_LEN - 2));
    if (sizeof(c) == 2) {
        send_bits(c >> 8, 8);
    }
    send_bits(c, 8);
    CS = 0;
}
