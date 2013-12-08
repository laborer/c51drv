/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "rom9346.h"
#include "spi.h"

#include "uart.h"
#include "print.h"


#define CS              ROM9346_CS
#define ADDR_LEN        ROM9346_ADDR_LEN
#define word_t          rom9346_word_t


#define MAKECMD(opcode, addr)                                   \
    (((4 | (opcode)) << ADDR_LEN) | (addr))


/* Send an instruction to the chip */
static void send_cmd(unsigned int cmd)
{
    /* Clear CS first to reset the internal control logic */
    CS = 0;
    DELAY_US(1);
    spi_init();

    /* Start conversation */
    CS = 1;
    /* Check busy status */
    while (!spi_recv());

    spi_send(cmd >> 8);
    spi_send(cmd);
}

/* Send a word to the chip */
static void send_word(word_t w)
{
    if (sizeof(word_t) == 2) {
        spi_send(w >> 8);
    }
    spi_send(w);
}

/* Receive a word from the chip */
static word_t recv_word()
{
    if (sizeof(word_t) == 2) {
        return (spi_recv() << 8) | spi_recv();
    }

    return spi_recv();
}

/* Enable erase and write. */
void rom9346_write_enable()
{
    send_cmd(MAKECMD(0, 3 << (ADDR_LEN - 2)));
    CS = 0;
}

/* Disable erase and write. */
void rom9346_write_disable()
{
    send_cmd(MAKECMD(0, 0 << (ADDR_LEN - 2)));
    CS = 0;
}

/* Erase one word. */
void rom9346_erase(unsigned int addr)
{
    send_cmd(MAKECMD(3, addr));
    CS = 0;
}

/* Erase all. */
void rom9346_erase_all()
{
    send_cmd(MAKECMD(0, 2 << (ADDR_LEN - 2)));
    CS = 0;
}

/* Read one word from addr */
word_t rom9346_read(unsigned int addr)
{
    word_t c;

    send_cmd(MAKECMD(2, addr));
    c = recv_word();
    CS = 0;

    return c;
}

/* Read n words from addr to string p */
void rom9346_readstr(unsigned int addr, word_t __idata *p, unsigned char n)
{
    send_cmd(MAKECMD(2, addr));
    for (; n != 0; p++, n--) {
        *p = recv_word();
    }
    CS = 0;
}

/* Write one word c to addr */
void rom9346_write(unsigned int addr, word_t w)
{
    send_cmd(MAKECMD(1, addr));
    send_word(w);
    CS = 0;
}

/* Write c to all memory.  Note: this doesn't erase automatically. */
void rom9346_write_all(word_t w)
{
    send_cmd(MAKECMD(0, 1 << (ADDR_LEN - 2)));
    send_word(w);
    CS = 0;
}
