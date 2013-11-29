/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#ifndef __STC12C5A60S2_H_
#define __STC12C5A60S2_H_


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

/* __sfr __at (0xA8) IE; */
__sbit __at (0xAD) EADC;
__sbit __at (0xAE) ELVD;

/* __sfr __at (0xB8) IP; */
__sbit __at (0xBD) PADC;
__sbit __at (0xBE) PLVD;
__sbit __at (0xBF) PPCA;

__sfr __at (0X8E) AUXR;
__sfr __at (0XA2) AUXR1;
__sfr __at (0X8F) WAKE_CLKO;
__sfr __at (0X97) CLK_DIV;
__sfr __at (0XA1) BUS_SPEED;
__sfr __at (0XAF) IE2;
__sfr __at (0XB7) IPH;
__sfr __at (0XB5) IP2;
__sfr __at (0XB6) IPH2;

__sfr __at (0XC0) P4;
__sbit __at (0xC0) P4_0;
__sbit __at (0xC1) P4_1;
__sbit __at (0xC2) P4_2;
__sbit __at (0xC3) P4_3;
__sbit __at (0xC4) P4_4;
__sbit __at (0xC5) P4_5;
__sbit __at (0xC6) P4_6;
__sbit __at (0xC7) P4_7;

__sfr __at (0XC8) P5;
__sbit __at (0xC8) P5_0;
__sbit __at (0xC9) P5_1;
__sbit __at (0xCA) P5_2;
__sbit __at (0xCB) P5_3;
__sbit __at (0xCC) P5_4;
__sbit __at (0xCD) P5_5;
__sbit __at (0xCE) P5_6;
__sbit __at (0xCF) P5_7;

__sfr __at (0X94) P0M0;
__sfr __at (0X93) P0M1;
__sfr __at (0X92) P1M0;
__sfr __at (0X91) P1M1;
__sfr __at (0X9D) P1ASF;
__sfr __at (0X96) P2M0;
__sfr __at (0X95) P2M1;
__sfr __at (0XB2) P3M0;
__sfr __at (0XB1) P3M1;
__sfr __at (0XB4) P4M0;
__sfr __at (0XB3) P4M1;
__sfr __at (0XBB) P4SW;
__sfr __at (0XCA) P5M0;
__sfr __at (0XC9) P5M1;

__sfr __at (0XB9) SADEN;
__sfr __at (0XA9) SADDR;
__sfr __at (0X9A) S2CON;
__sfr __at (0X9B) S2BUF;
__sfr __at (0X9C) BRT;

__sfr __at (0XC1) WDT_CONTR;

__sfr __at (0XD8) CCON;
__sbit __at (0xD8) CCF0;
__sbit __at (0xD9) CCF1;
__sbit __at (0xDE) CR;
__sbit __at (0xDF) CF;

__sfr __at (0XD9) CMOD;
__sfr __at (0XE9) CL;
__sfr __at (0XF9) CH;

__sfr __at (0XDA) CCAPM0;
__sfr __at (0XDB) CCAPM1;
__sfr __at (0XEA) CCAP0L;
__sfr __at (0XFA) CCAP0H;
__sfr __at (0XEB) CCAP1L;
__sfr __at (0XFB) CCAP1H;

__sfr __at (0XF2) PCA_PWM0;
__sfr __at (0XF3) PCA_PWM1;

__sfr __at (0XBC) ADC_CONTR;
__sfr __at (0XBD) ADC_RES;
__sfr __at (0XBE) ADC_RESL;

__sfr __at (0XCE) SPCTL;
__sfr __at (0XCD) SPSTAT;
__sfr __at (0XCF) SPDAT;

__sfr __at (0XC2) IAP_DATA;
__sfr __at (0XC3) IAP_ADDRH;
__sfr __at (0XC4) IAP_ADDRL;

__sfr __at (0XC5) IAP_CMD;
__sfr __at (0XC6) IAP_TRIG;
__sfr __at (0XC7) IAP_CONTR;


#endif /* __STC12C5A60S2_H_ */
