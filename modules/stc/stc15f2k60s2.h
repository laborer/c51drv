#ifndef __STC15F2K60S2_H_
#define __STC15F2K60S2_H_


#include <8052.h>


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

__sfr __at (0XE8) P6;
__sbit __at (0xE8) P6_0;
__sbit __at (0xE9) P6_1;
__sbit __at (0xEA) P6_2;
__sbit __at (0xEB) P6_3;
__sbit __at (0xEC) P6_4;
__sbit __at (0xED) P6_5;
__sbit __at (0xEE) P6_6;
__sbit __at (0xEF) P6_7;

__sfr __at (0XF8) P7;
__sbit __at (0xF8) P7_0;
__sbit __at (0xF9) P7_1;
__sbit __at (0xFA) P7_2;
__sbit __at (0xFB) P7_3;
__sbit __at (0xFC) P7_4;
__sbit __at (0xFD) P7_5;
__sbit __at (0xFE) P7_6;
__sbit __at (0xFF) P7_7;

__sfr __at (0X94) P0M0;
__sfr __at (0X93) P0M1;
__sfr __at (0X92) P1M0;
__sfr __at (0X91) P1M1;
__sfr __at (0X96) P2M0;
__sfr __at (0X95) P2M1;
__sfr __at (0XB2) P3M0;
__sfr __at (0XB1) P3M1;
__sfr __at (0XB4) P4M0;
__sfr __at (0XB3) P4M1;
__sfr __at (0XCA) P5M0;
__sfr __at (0XC9) P5M1;
__sfr __at (0XCC) P6M0;
__sfr __at (0XCB) P6M1;
__sfr __at (0XE2) P7M0;
__sfr __at (0XE1) P7M1;

__sfr __at (0X8E) AUXR;
__sfr __at (0XA2) AUXR1;
__sfr __at (0XA2) P_SW1;
__sfr __at (0X97) CLK_DIV;
__sfr __at (0XA1) BUS_SPEED;
__sfr __at (0X9D) P1ASF;
__sfr __at (0XBA) P_SW2;

/* __sfr __at (0xA8) IE; */
__sbit __at (0xAD) EADC;
__sbit __at (0xAE) ELVD;

/* __sfr __at (0xB8) IP; */
__sbit __at (0xBD) PADC;
__sbit __at (0xBE) PLVD;
__sbit __at (0xBF) PPCA;

__sfr __at (0XAF) IE2;
__sfr __at (0XB5) IP2;
__sfr __at (0X8F) INT_CLKO;

__sfr __at (0XD1) T4T3M;
__sfr __at (0XD1) T3T4M;
__sfr __at (0XD2) T4H;
__sfr __at (0XD3) T4L;
__sfr __at (0XD4) T3H;
__sfr __at (0XD5) T3L;
__sfr __at (0XD6) T2H;
__sfr __at (0XD7) T2L;
__sfr __at (0XAA) WKTCL;
__sfr __at (0XAB) WKTCH;
__sfr __at (0XC1) WDT_CONTR;

__sfr __at (0X9A) S2CON;
__sfr __at (0X9B) S2BUF;
__sfr __at (0XAC) S3CON;
__sfr __at (0XAD) S3BUF;
__sfr __at (0X84) S4CON;
__sfr __at (0X85) S4BUF;
__sfr __at (0XA9) SADDR;
__sfr __at (0XB9) SADEN;

__sfr __at (0XBC) ADC_CONTR;
__sfr __at (0XBD) ADC_RES;
__sfr __at (0XBE) ADC_RESL;

__sfr __at (0XCD) SPSTAT;
__sfr __at (0XCE) SPCTL;
__sfr __at (0XCF) SPDAT;

__sfr __at (0XC2) IAP_DATA;
__sfr __at (0XC3) IAP_ADDRH;
__sfr __at (0XC4) IAP_ADDRL;
__sfr __at (0XC5) IAP_CMD;
__sfr __at (0XC6) IAP_TRIG;
__sfr __at (0XC7) IAP_CONTR;

__sfr __at (0XD8) CCON;
__sbit __at (0xD8) CCF0;
__sbit __at (0xD9) CCF1;
__sbit __at (0xDA) CCF2;
__sbit __at (0xDE) CR;
__sbit __at (0xDF) CF;

__sfr __at (0XD9) CMOD;
__sfr __at (0XE9) CL;
__sfr __at (0XF9) CH;
__sfr __at (0XDA) CCAPM0;
__sfr __at (0XDB) CCAPM1;
__sfr __at (0XDC) CCAPM2;
__sfr __at (0XEA) CCAP0L;
__sfr __at (0XEB) CCAP1L;
__sfr __at (0XEC) CCAP2L;
__sfr __at (0XF2) PCA_PWM0;
__sfr __at (0XF3) PCA_PWM1;
__sfr __at (0XF4) PCA_PWM2;
__sfr __at (0XFA) CCAP0H;
__sfr __at (0XFB) CCAP1H;
__sfr __at (0XFC) CCAP2H;


#endif /* __STC15F2K60S2_H_ */
