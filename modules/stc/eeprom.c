/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "../common.h"
#include "stc.h"
#include "eeprom.h"


#if defined __STC89C5xRC_RDP_H_                                 \
    || defined __STC90C5xAD_H_                                  \

#  if FOSC > 20000000L
#    define ENABLE_IAP  0x80
#  elif FOSC > 10000000L
#    define ENABLE_IAP  0x81
#  elif FOSC > 5000000L
#    define ENABLE_IAP  0x82
#  else
#    define ENABLE_IAP  0x83
#  endif

#  define TRIG_CMD1     0x46
#  define TRIG_CMD2     0xB9

#elif defined __STC12C5630AD_H_                                 \
    || defined __STC12C5410AD_H_                                \
    || defined __STC12C2052AD_H_                                \

#  if FOSC > 24000000L
#    define ENABLE_IAP  0x80
#  elif FOSC > 20000000L
#    define ENABLE_IAP  0x81
#  elif FOSC > 12000000L
#    define ENABLE_IAP  0x82
#  elif FOSC > 6000000L
#    define ENABLE_IAP  0x83
#  elif FOSC > 3000000L
#    define ENABLE_IAP  0x84
#  elif FOSC > 2000000L
#    define ENABLE_IAP  0x85
#  elif FOSC > 1000000L
#    define ENABLE_IAP  0x86
#  else
#    define ENABLE_IAP  0x87
#  endif

#  define TRIG_CMD1     0x46
#  define TRIG_CMD2     0xB9

#elif defined __STC12C5A60S2_H_                                 \
    || defined __STC15F2K60S2_H_                                \
    || defined __STC15F104E_H_

#  if FOSC > 24000000L
#    define ENABLE_IAP  0x80
#  elif FOSC > 20000000L
#    define ENABLE_IAP  0x81
#  elif FOSC > 12000000L
#    define ENABLE_IAP  0x82
#  elif FOSC > 6000000L
#    define ENABLE_IAP  0x83
#  elif FOSC > 3000000L
#    define ENABLE_IAP  0x84
#  elif FOSC > 2000000L
#    define ENABLE_IAP  0x85
#  elif FOSC > 1000000L
#    define ENABLE_IAP  0x86
#  else
#    define ENABLE_IAP  0x87
#  endif

#  define TRIG_CMD1     0x5A
#  define TRIG_CMD2     0xA5

#endif


static void disable(void)
{
    /* Disable IAP/ISP. */
    IAP_CONTR = 0;
    /* Clear command register. */
    IAP_CMD = 0;
    /* Clear Trigger. */
    IAP_TRIG = 0;
    /* Point address to non-eeprom region. */
    IAP_ADDRH = 0xFF;
    IAP_ADDRL = 0xFF;
}

void eeprom_erase(uint16_t addr)
{
    IAP_CONTR = ENABLE_IAP;
    /* ERASE Command */
    IAP_CMD = 3; 
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = TRIG_CMD1;
    IAP_TRIG = TRIG_CMD2;
    nop();
    disable();
}

void eeprom_write(uint16_t addr, uint8_t c)
{
    IAP_CONTR = ENABLE_IAP;
    /* PROGRAM Command */
    IAP_CMD = 2; 
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_DATA = c;
    IAP_TRIG = TRIG_CMD1;
    IAP_TRIG = TRIG_CMD2;
    nop();
    disable();
}

uint8_t eeprom_read(uint16_t addr)
{
    IAP_CONTR = ENABLE_IAP;
    /* READ Command */
    IAP_CMD = 1; 
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = TRIG_CMD1;
    IAP_TRIG = TRIG_CMD2;
    nop();
    /* disable(); */

    return IAP_DATA;
}
