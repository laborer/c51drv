#ifndef __STC12C5630AD_H_
#define __STC12C5630AD_H_


#include <8052.h>


/* __sfr __at (0x90) P1; */
__sbit __at (0x90) T2;
__sbit __at (0x91) T2EX;

__sfr __at (0XE1) WDT_CONTR;
__sfr __at (0XE2) ISP_DATA;
__sfr __at (0XE3) ISP_ADDRH;
__sfr __at (0XE4) ISP_ADDRL;
__sfr __at (0XE5) ISP_CMD;
__sfr __at (0XE6) ISP_TRIG;
__sfr __at (0XE7) ISP_CONTR;

/* __sfr __at (0xA0) P2; */
__sbit __at (0xA4) CEX3;
__sbit __at (0xA0) CEX2;

/* __sfr __at (0xB0) P3; */
__sbit __at (0xB5) CEX1;
__sbit __at (0xB7) CEX0;
__sbit __at (0xB4) ECI;

/* __sfr __at (0xB8) IP; */
__sbit __at (0xBE) PPCA_LVD;
__sbit __at (0xBD) PADC_SPI;

/* __sfr __at (0xA8) IE; */
__sbit __at (0xAD) EADC_SPI;
__sbit __at (0xAE) EPCA_LVD;

__sfr __at (0XC7) CLK_DIV;

__sfr __at (0X93) P0M0;
__sfr __at (0X94) P0M1;
__sfr __at (0X91) P1M0;
__sfr __at (0X92) P1M1;
__sfr __at (0X95) P2M0;
__sfr __at (0X96) P2M1;
__sfr __at (0XB1) P3M0;
__sfr __at (0XB2) P3M1;

__sfr __at (0X84) SPSTAT;
__sfr __at (0X85) SPCTL;
__sfr __at (0X86) SPDAT;

__sfr __at (0XC5) ADC_CONTR;
__sfr __at (0XC6) ADC_DATA;
__sfr __at (0XBE) ADC_LOW2;

__sfr __at (0XD8) CCON;
__sbit __at (0xD8) CCF0;
__sbit __at (0xD9) CCF1;
__sbit __at (0xDA) CCF2;
__sbit __at (0xDB) CCF3;
__sbit __at (0xDC) CCF4;
__sbit __at (0xDD) CCF5;
__sbit __at (0xDE) CR;
__sbit __at (0xDF) CF;

__sfr __at (0XD9) CMOD;
__sfr __at (0XDA) CCAPM0;
__sfr __at (0XDB) CCAPM1;
__sfr __at (0XDC) CCAPM2;
__sfr __at (0XDD) CCAPM3;
__sfr __at (0XDE) CCAPM4;
__sfr __at (0XDF) CCAPM5;

__sfr __at (0XE9) CL;
__sfr __at (0XEA) CCAP0L;
__sfr __at (0XEB) CCAP1L;
__sfr __at (0XEC) CCAP2L;
__sfr __at (0XED) CCAP3L;
__sfr __at (0XEE) CCAP4L;
__sfr __at (0XEF) CCAP5L;

__sfr __at (0XF9) CH;
__sfr __at (0XFA) CCAP0H;
__sfr __at (0XFB) CCAP1H;
__sfr __at (0XFC) CCAP2H;
__sfr __at (0XFD) CCAP3H;
__sfr __at (0XFE) CCAP4H;
__sfr __at (0XFF) CCAP5H;

__sfr __at (0XF2) PCA_PWM0;
__sfr __at (0XF3) PCA_PWM1;
__sfr __at (0XF4) PCA_PWM2;
__sfr __at (0XF5) PCA_PWM3;
__sfr __at (0XF6) PCA_PWM4;
__sfr __at (0XF7) PCA_PWM5;


#endif /* __STC12C5630AD_H_ */
