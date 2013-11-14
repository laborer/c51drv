/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"


void uartint(void) __interrupt 4 __using 2
{
    P0_6 = 1;

    if (TI) {
        TI = 0;
        SBUF = 'A';
    }
}

void init(void)
{
    /* Set Timer1 mode */
    /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00
       0     0     1   0   -     -     -   -   */
    TMOD &= ~T1_MASK;
    TMOD |= T1_M1;

    /* Set ratio for baud rate and oscillator frequency */
    /* SMOD1 SMOD0 - POF GF1 GF0 PD IDL
       0     -     - -   -   -   -  -   */
    PCON &= ~SMOD;

    /* Set timer */
    /* 256 - FOSC * (SMOD1 + 1) / BAUD / 32 / 12 */ 
    TH1 = 256 - 12000000L * (0 + 1) / 2400 / 32 / 12;

    /* Start Timer1 */
    TR1 = 1;

    /* SM0 SM1 SM2 REN TB8 RB8 TI RI
       0   1   0   1   0   0   0  0  */
    SCON = 0x50;

    /* Enable UART interrupt */
    ES = 1;
    /* Enable global interrupt */
    EA = 1;

    TI = 1;
}


void main(void)
{
    init();
    
    while(1);
}
