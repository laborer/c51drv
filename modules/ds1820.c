/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "ds1820.h"


#if defined SDCC || defined __SDCC
#pragma callee_saves send_bit,send_byte,recv_bit,recv_byte
#endif


#define DQ DS1820_DQ


static uint8_t crc;


/* Send bit b */
static void send_bit(__bit b)
{
    DQ = 0;
    DELAY_US(10);
    DQ = b;
    DELAY_US(60);
    DQ = 1;
    DELAY_US(1);
}

/* Send byte c */
static void send_byte(uint8_t c)
{
    uint8_t i;

    for (i = 8; i != 0; i--) {
        send_bit(c & 1);
        c >>= 1;
    }
}

/* Receive a bit */
static __bit recv_bit(void)
{
    __bit b;

    DQ = 0;
    DELAY_US(1);
    DQ = 1;
    DELAY_US(10);
    b = DQ;
    DELAY_US(70);

    return b;
}

/* Receive a byte */
static uint8_t recv_byte(void)
{
    uint8_t     i;
    uint8_t     c;

    c = 0;
    for (i = 8; i != 0; i--) {
        c >>= 1;
        if (recv_bit()) {
            c |= 0x80;
        }
    }

    return c;
}

/* Initialize conversation */
static int8_t init(void)
{
    /* It seems that if DQ is not pulled-up long enough, the chip
       doesn't respond correctly.  This is not described in
       the datasheet. */
    DQ = 1;
    delay_ms(2);
    DQ = 0;
    DELAY_US(500);
    DQ = 1;
    DELAY_US(65);
    if (DQ) {
        return DS1820_ERR_INIT;
    }
    DELAY_US(500 - 65);
    if (!DQ) {
        return DS1820_ERR_INIT;
    }
    return 0;
}

/* Send out the address of the device we are looking for */
static void match_rom(uint8_t __idata *rom)
{
    uint8_t i;

    if (rom) {
        /* Match ROM */
        send_byte(0x55);
        for (i = 8; i != 0; rom++, i--) {
            send_byte(*rom);
        }
    } else {
        /* Skip ROM */
        send_byte(0xCC);
    }
}

/* Find the device addressed next to rom on the bus.  Parameter cmd
   decides whether it is a normal or alarm search */
static int8_t next_rom(uint8_t __idata *rom, uint8_t cmd)
{
    int8_t      i;
    int8_t      j;
    int8_t      k;
    int8_t      b;
    int8_t      k0;

    /* Initialize DS1820. */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    /* Increase parameter rom by one, as we want to find the chip
       whose ROM is right next to rom. */
    i = 8;
    do {
        i -= 1;
        rom[i] = REVERSE(REVERSE(rom[i]) + 1);
    } while (i > 0 && rom[i] == 0);

    /* Overflow!  There is no next rom. */
    if (i == 0 && rom[i] == 0) {
        return 1;
    }

    /* Search ROM or Alarm Search */
    send_byte(cmd);

    /* Trace along the path as indicated by rom until no chip is on
       the way. */
    k0 = -1;
    for (k = 0, i = 0, j = 1; k < 64; k++, j <<= 1) {
        if (j == 0) {
            j = 1;
            i += 1;
        }

        /* Poll chips on both branches. */
        b = recv_bit();
        b |= (int8_t)recv_bit() << 1;

        /* Follow the path given by rom. */
        send_bit(rom[i] & j);

        /* No chips on this node?  Then who responded eariler? */
        if (b == 3) {
            return DS1820_ERR_SEARCH;
        }
        /* rom asks us to go to the left branch while there are chips
           on the right.  Record the position (depth of the node) so
           we can go back to the right branch later if neccessary. */
        if (b < 2 && (rom[i] & j) == 0) {
            k0 = k;
        }
        /* There is no way to go.  There is no chip on the side where
           rom asks us to go. */
        if (b == 2 && (rom[i] & j) != 0 || b == 1 && (rom[i] & j) == 0) {
            break;
        }
    }

    /* If rom is a valid chip, return 0; or, if we haven't seen a
       valid right branch with chips on it, there is no chip next to
       rom. */
    if (k == 64) {
        return 0;
    } else if (k0 < 0) {
        return 2;
    }

    /* Initialize DS1820 again to do the second search */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    /* Search ROM or Alarm Search*/
    send_byte(cmd);

    /* Tracing along the patch as indicated by rom until the place
       where we saw the last valid right branch. */
    for (k = 0, i = 0, j = 1; k <= k0; k++, j <<= 1) {
        if (j == 0) {
            j = 1;
            i += 1;
        }

        /* If we reach to the last valid right branch, we go right
           instead of left as indicated by rom. */
        if (k == k0) {
            rom[i] |= j;
        }

        /* We know that we're on a path to a valid chip, so we don't
           need to care chip polling results. */
        recv_bit();
        recv_bit();

        /* Follow the path given by rom. */
        send_bit(rom[i] & j);
    }

    /* Now let's find a path to the left-most chip. */
    for (; k < 64; k++, j <<= 1) {
        if (j == 0) {
            j = 1;
            i += 1;
        }

        /* Poll chips on both branches. */
        b = recv_bit();
        b |= (int8_t)recv_bit() << 1;

        /* Always go left as long as there is a chip on the left. */
        if (b == 3) {
            return DS1820_ERR_SEARCH;
        } else if (b == 1) {
            rom[i] |= j;
        } else {
            rom[i] &= ~j;
        }

        /* After updating rom, we follow the path given by it. */
        send_bit(rom[i] & j);
    }

    return 0;
}

/* Read the n and n+1-th byte on the scratchpad */
static uint16_t read_scratchpad(uint8_t n)
{
    uint8_t     i;
    uint8_t     c;
    uint16_t    ret;

    /* Read Scratchpad */
    send_byte(0xBE);

    crc = 0;
    ret = 0;
    for (i = 0; i < 9; i++) {
        c = recv_byte();
        crc = crc8(crc, c);

        if (i == n) {
            ret = c;
        } else if (i == n + 1) {
            ret |= (uint16_t)c << 8;
        }
    }

    return ret;
}

/* Write threshold th, tl and configuration cfg to the scratchpad */
static void write_scratchpad(uint8_t th,
                             uint8_t tl,
                             uint8_t cfg)
{
    /* Write Scratchpad */
    send_byte(0x4E);

    send_byte(th);
    send_byte(tl);
    send_byte(cfg);
}


/* Read the address of a device */
int8_t ds1820_read_rom(uint8_t __idata *rom)
{
    uint8_t i;

    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    /* Read ROM */
    send_byte(0x33);

    crc = 0;
    for (i = 8; i != 0; rom++, i--) {
        *rom = recv_byte();
        crc = crc8(crc, *rom);
    }
    if (crc != 0) {
        return DS1820_ERR_CRC;
    }

    return 0;
}

/* Read which power supply a device uses */
int8_t ds1820_read_power_supply(uint8_t __idata *rom)
{
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    /* Read Power Supply */
    send_byte(0xB4);

    return (recv_bit()) ? 1 : 0;
}

/* Send command to start temperature measurement */
int8_t ds1820_convert_t(uint8_t __idata *rom)
{
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    /* Convert T */
    send_byte(0x44);

    return 0;
}

/* Store scratchpad to eeprom */
int8_t ds1820_copy_scratchpad(uint8_t __idata *rom)
{
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    /* Copy Scratchpad */
    send_byte(0x48);

    return 0;
}

/* Reload scratchpad from eeprom */
int8_t ds1820_recall_e2(uint8_t __idata *rom)
{
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    /* Recall E^2 */
    send_byte(0xB8);

    return 0;
}

/* Search the device addressed next to rom */
int8_t ds1820_search_rom(uint8_t __idata *rom)
{
    /* Search ROM */
    return next_rom(rom, 0xF0);
}

/* Search the device addressed next to rom, which triggered its
   alarm */
int8_t ds1820_alarm_search(uint8_t __idata *rom)
{
    /* Alarm Search */
    return next_rom(rom, 0xEC);
}

/* Read the measured temperature */
int16_t ds1820_read_temperature(uint8_t __idata *rom)
{
    uint16_t ret;

    if (init() < 0) {
        return 0x80FF & DS1820_ERR_INIT;
    }

    match_rom(rom);

    ret = read_scratchpad(0);
    if (crc) {
        return 0x80FF & DS1820_ERR_CRC;
    }

    return ret;
}

/* Read the resolution of temperature measurement */
int8_t ds1820_read_resolution(uint8_t __idata *rom)
{
    uint16_t ret;

    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    ret = read_scratchpad(4);
    if (crc) {
        return DS1820_ERR_CRC;
    }

    return (((int8_t)ret & 0x7F) >> 5) + 9;
}

/* Set the resolution of temperature measurement */
int8_t ds1820_write_resolution(uint8_t __idata *rom, uint8_t res)
{
    uint16_t ret;

    /* The first command is to save trigger settings. */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    ret = read_scratchpad(2);
    if (crc) {
        return DS1820_ERR_CRC;
    }

    /* The second command is to write trigger and resoltuion
       settings. */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    write_scratchpad(ret, ret >> 8, (res - 9) << 5);

    return 0;
}

/* Read trigger temperatures */
int8_t ds1820_read_triggers(uint8_t __idata *rom,
                            uint8_t __idata *th,
                            uint8_t __idata *tl)
{
    uint16_t ret;

    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    ret = read_scratchpad(2);
    if (crc) {
        return DS1820_ERR_CRC;
    }

    *th = ret;
    *tl = ret >> 8;

    return 0;
}

/* Set trigger temperatures */
int8_t ds1820_write_triggers(uint8_t __idata *rom,
                             uint8_t th,
                             uint8_t tl)
{
    uint8_t ret;

    /* The first command is to save resolution setting. */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    ret = read_scratchpad(4);
    if (crc) {
        return DS1820_ERR_CRC;
    }

    /* The second command is to write trigger and resoltuion
       settings. */
    if (init() < 0) {
        return DS1820_ERR_INIT;
    }

    match_rom(rom);

    write_scratchpad(th, tl, ret);

    return 0;
}

/* This routine is to demonstrate how to wait for the temperature
   conversion properly.  Since the control flow stops in the routine
   for about one second in the worst case scenario, it is not
   recommended to use it in production code with an event loop.  For
   such case, a better way is to run convert_t(...) first, then return
   the control flow to the event loop and call read_temperature(...)
   one second later (for 12-bit resolution). */
int16_t ds1820_measure_temperature(uint8_t __idata *rom)
{
    uint8_t     pwr;
    uint8_t     res;
    uint8_t     ret;
    uint8_t     i;

    /* Read power status.  If the chip is powered, we can use read
       slot to test when it finishes conversion after issuing a
       "Convert T" command.  However, if the chip is in parasite power
       mode, using read slot interrupts temperature conversions, thus
       we need to estimate the maximum conversion time using the
       thermometer resolution configuration. */
    pwr = ds1820_read_power_supply(rom);
    if (pwr < 0) {
        return 0x80FF & pwr;
    }

    /* Read resolution configuration if the chip uses parasite
       power. */
    res = 0;
    if (pwr == 0) {
        res = ds1820_read_resolution(rom);
        if (res < 0) {
            return 0x80FF & res;
        }
    }

    /* Let's "Convert T" */
    ret = ds1820_convert_t(rom);
    if (ret < 0) {
        return 0x80FF & ret;
    }

    /* The higher resolution, the longer does it take to do the
       conversion. */
    if (pwr == 0) {
        delay_ms(94 << (res - 9));
    } else {
        /* If the chip hasn't responded with a finish signal in 1
           second, return timeout error. */
        for (i = 100; i != 0 && !recv_bit(); i--) {
            delay_ms(10);
        }
        if (i == 0) {
            return 0x80FF & DS1820_ERR_TIMEOUT;
        }
    }

    /* Read the converted temperature from sratchpad and return it to
       user. */
    return ds1820_read_temperature(rom);
}
