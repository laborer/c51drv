/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __STC_H
#define __STC_H


#include "modeldb.h"

#if defined(TARGET_FAMILY_STC89C)

#include "stc89c5xrc_rdp.h"

#elif defined(TARGET_FAMILY_STC12C52) \
    || defined(TARGET_FAMILY_STC12C5A) \
    || defined(TARGET_FAMILY_STC10F) \
    || defined(TARGET_FAMILY_STC11F)

#include "stc12c5a60s2.h"

#endif /* TARGET_FAMILY_x */


#endif /* __STC_H */
