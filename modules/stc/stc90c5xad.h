/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __STC90C5xAD_H_
#define __STC90C5xAD_H_


#include <8052.h>


/* __sfr __at (0xC8) T2CON; */
__sbit __at (0xC8) CP_RL2;
__sbit __at (0xC9) C_T2;
__sbit __at (0xCA) TR2;
__sbit __at (0xCB) EXEN2;
__sbit __at (0xCC) TCLK;
__sbit __at (0xCD) RCLK;
__sbit __at (0xCE) EXF2;
__sbit __at (0xCF) TF2;

/* __sfr __at (0x90) P1; */
__sbit __at (0x90) T2;
__sbit __at (0x91) T2EX;

__sfr __at (0xC0) P4;
__sbit __at (0xC0) P40;
__sbit __at (0xC1) P41;
__sbit __at (0xC2) P42;
__sbit __at (0xC3) P43;

__sfr __at (0xE8) XICON;
__sfr __at (0xE1) WDT_CONTR;
__sfr __at (0XE2) ISP_DATA;
__sfr __at (0XE3) ISP_ADDRH;
__sfr __at (0XE4) ISP_ADDRL;
__sfr __at (0XE5) ISP_CMD;
__sfr __at (0XE6) ISP_TRIG;
__sfr __at (0XE7) ISP_CONTR;

__sfr __at (0X8E) AUXR;
__sfr __at (0XA2) AUXR1;
__sfr __at (0XA9) SADDR;
__sfr __at (0XB7) IPH;
__sfr __at (0XB9) SADEN;
__sfr __at (0XC9) T2MOD;


#endif /* __STC90C5xAD_H_ */
