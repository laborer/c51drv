/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "rom9346.h"
#include "spi.h"

#include "uart.h"
#include "print.h"


#define CS      ROM9346_CS
#define ADDRLEN ROM9346_ADDRLEN
#define word_t  rom9346_word_t


#define BUILDCMD(opcode)                                        \
    (BIN(1##opcode) << (ADDRLEN + 2 - LOG2_8(BIN(1##opcode))))


/* Send an instruction to the chip */
static void send_cmd(uint16_t cmd)
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
static word_t recv_word(void)
{
    if (sizeof(word_t) == 2) {
        return (spi_recv() << 8) | spi_recv();
    }

    return spi_recv();
}

/* Enable erase and write. */
void rom9346_write_enable(void)
{
    send_cmd(BUILDCMD(0011));
    CS = 0;
}

/* Disable erase and write. */
void rom9346_write_disable(void)
{
    send_cmd(BUILDCMD(0000));
    CS = 0;
}

/* Erase one word. */
void rom9346_erase(uint16_t addr)
{
    send_cmd(BUILDCMD(11) | addr);
    CS = 0;
}

/* Erase all. */
void rom9346_erase_all(void)
{
    send_cmd(BUILDCMD(0010));
    CS = 0;
}

/* Read one word from addr */
word_t rom9346_read(uint16_t addr)
{
    word_t c;

    send_cmd(BUILDCMD(10) | addr);
    c = recv_word();
    CS = 0;

    return c;
}

/* Read n words from addr to string p */
void rom9346_readstr(uint16_t addr, word_t __idata *p, uint8_t n)
{
    send_cmd(BUILDCMD(10) | addr);
    for (; n != 0; p++, n--) {
        *p = recv_word();
    }
    CS = 0;
}

/* Write one word c to addr */
void rom9346_write(uint16_t addr, word_t w)
{
    send_cmd(BUILDCMD(01) | addr);
    send_word(w);
    CS = 0;
}

/* Write c to all memory.  Note: this doesn't erase automatically. */
void rom9346_write_all(word_t w)
{
    send_cmd(BUILDCMD(0001));
    send_word(w);
    CS = 0;
}
