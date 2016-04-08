/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __STC_H
#define __STC_H


#include "modeldb.h"


#if defined TARGET_FAMILY_STC89C                                \
    || defined TARGET_FAMILY_STC90CxR

#include "stc89c5xrc_rdp.h"

#elif defined TARGET_FAMILY_STC90CxAD

#include "stc90c5xad.h"

#elif defined TARGET_FAMILY_STC12Cx052

#include "stc12c2052ad.h"

#elif defined TARGET_FAMILY_STC12C56

#include "stc12c5630ad.h"

#elif defined TARGET_FAMILY_STC12C54

#include "stc12c5410ad.h"

#elif defined TARGET_FAMILY_STC12C52                            \
    || defined TARGET_FAMILY_STC12C5A                           \
    || defined TARGET_FAMILY_STC10F                             \
    || defined TARGET_FAMILY_STC11F

#include "stc12c5a60s2.h"

#elif defined TARGET_FAMILY_STC15F10

#include "stc15f104e.h"

#elif defined TARGET_FAMILY_STC15F

#include "stc15f2k60s2.h"

#endif /* TARGET_FAMILY_x */


#if defined __STC89C5xRC_RDP_H_                                 \
    || defined __STC90C5xAD_H_                                  \
    || defined __STC12C5630AD_H_                                \
    || defined __STC12C5410AD_H_                                \
    || defined __STC12C2052AD_H_

#  define IAP_DATA      ISP_DATA
#  define IAP_ADDRH     ISP_ADDRH
#  define IAP_ADDRL     ISP_ADDRL
#  define IAP_CMD       ISP_CMD
#  define IAP_TRIG      ISP_TRIG
#  define IAP_CONTR     ISP_CONTR

#endif /* __STC89C5XRC_RDP_H_ */


/* Reboot to ISP section */
/* IAPEN SWBS SWRST CMD_FAIL - WT2 WT1 WT0 */
#define STC_REBOOT_ISP()                                        \
    do {                                                        \
        IAP_CONTR = 0x60;                                       \
    } while (0)


#endif /* __STC_H */
