/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "uart.h"


/* Ring buffer struct for UART I/O */
struct buffer
{
    unsigned char dat[4];
    unsigned char in;
    unsigned char out;
};


/* Transmitting buffer */
struct buffer txbuf;
/* Receiving buffer */
struct buffer rcbuf;

/* Transmitting is turned off */
char txoff;
/* Receiving buffer is overflow */
char rcoff;


/* UART interrupt routine */
void uart_interrupt(void) __interrupt SI0_VECTOR __using 1
{
    /* Check if UART is ready to read */
    if (RI) {
        /* Reset receiving interrupt flag */
        RI = 0;
        /* Check if receiving buffer is full */
        if (BUF_FULL(rcbuf)) {
            /* Set buffer-is-full flag */
            rcoff = 1;
        } else {
            /* Read one byte from SBUF and put it in receiving
               buffer */
            BUF_PUT(rcbuf, SBUF);
        }
    }
    /* Check if UART is ready to write */
    if (TI) {
        /* Reset transmitting interrupt flag */
        TI = 0;
        /* Check if transmitting buffer is empty */
        if (BUF_EMPTY(txbuf)) {
            /* Turn off transmitting */
            txoff = 1;
        } else {
            /* Read a byte from transmitting buffer and put it in
               SBUF */
            SBUF = BUF_GET(txbuf);
        }
    }
}

/* Check if receiving buffer is empty */
char uart_rcempty(void)
{
    return BUF_EMPTY(rcbuf);
}

/* Read a byte from receiving buffer */
int uart_rcget(void)
{
    if (BUF_EMPTY(rcbuf)) {
        return -1;
    }
    return BUF_GET(rcbuf);
}

/* Send a byte to transmitting buffer */
char uart_txput(unsigned char c)
{
    if (BUF_FULL(txbuf)) {
        return -1;
    }

    BUF_PUT(txbuf, c);
    /* Check if transmitting is turned off */
    if (txoff) {
        /* Turn on transmitting */
        txoff = 0;
        /* Set transmitting interrupt flag to be ready */
        TI = 1;
    }
    return 0;
}

/* Send a byte in block mode */
void uart_putchar(unsigned char c)
{
    while (uart_txput(c)) {
        /* Enter idle mode */
        PCON |= IDL;
    }
}

/* Read a byte in block mode */
unsigned char uart_getchar(void)
{
    while (uart_rcempty()) {
        /* Enter idle mode */
        PCON |= IDL;
    }
    return uart_rcget();
}

/* Set a fixed baudrate */
void uart_baudrate(void)
{
    /* Select clock source */
    /* TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2# CP/RL2#
       -   -    0    0    -     -   -     -       */
    // T2CON &= 0xCF;
    /* - - - BRR TBCK RBCK SPD SRC 
       - - - -   0    0    -   -   */
    // BDRCON &= 0xF3;

    /* Set Timer1 mode */
    /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00
       0     0     1   0   -     -     -   -   */
    TMOD &= ~T1_MASK;
    TMOD |= T1_M1;

    /* Set ratio for baud rate and oscillator frequency */
    /* SMOD1 SMOD0 - POF GF1 GF0 PD IDL
       0     -     - -   -   -   -  -   */
    PCON |= SMOD;

    /* Set timer */
    /* 256 - FOSC * (SMOD1 + 1) / BAUD / 32 / 12 */ 
    TH1 = 256 - FOSC * (1 + 1) / UART_BAUD / 32 / TICKS;

    /* Start Timer1 */
    TR1 = 1;
}

/* Detect and set baudrate automatically  */
char uart_baudrate_auto(void)
{
    unsigned int        t;
    unsigned int        tmax;
    unsigned int        tmin;
    unsigned char       i;
    unsigned char       k;

    /* Disable Timer1 interrupt */
    ET1 = 0;
    /* Disable UART interrupt */
    ES = 0;

    /* Set Timer1 to be a 16-bit timer */
    /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00
       0     0     0   1   -     -     -   -   */
    TMOD &= ~T1_MASK;
    TMOD |= T1_M0;

    /* Pause Timer1 */
    TR1 = 0;

    tmax = 0;
    tmin = 0xFFFF;
    /* k stores the number of successfully captured low level
       interval */
    k = 0;

    for (i = 0; i < 200; i++) {
        /* Wait RXD to be high */
        while (!RXD);
        /* Reset Timer1 */
        TL1 = 0;
        TH1 = 0;
        /* Reset Timer1 overflow flag */
        TF1 = 0;
        /* Resume Timer1 */
        TR1 = 1;
        /* Wait for a falling edge of RXD before Timer1 overflows */
        while (RXD && !TF1);
        /* Reset Timer1 */
        TL1 = 0;
        TH1 = 0;
        /* Restart if it waited for too long */
        if (TF1) {
            continue;
        }
        /* Wait for a rising edge of RXD */
        while (!RXD);
        /* Pause Timer1 */
        TR1 = 0;
        /* The baudrate is too low if Timer1 overflows */
        if (TF1) {
            return -1;
        }

        /* Now we captured one low level interval */
        k += 1;

        /* Assume SMOD1=1 */
        /* t = (TH1 * 256 + TL1 + 8) / 16; */
        t = (TH1 * 256 + TL1 + 8) >> 4;
        if (t > tmax) {
            tmax = t;
        }
        if (t > tmin && t < tmin * 2) {
            t -= tmin;
        }
        if (t < tmin) {
            tmin = t;
        }
        if (tmax >= tmin * 4 + tmin) {
            k = 0xFF;
            break;
        }
    }

    if (tmin >> 8) {
        return -1;
    }
    if (k < 3) {
        return -2;
    }

    /* Set Timer1 to be an 8-bit auto-reload timer */
    /* GATE1 C/T1# M11 M01 GATE0 C/T0# M10 M00
       0     0     1   0   -     -     -   -   */
    TMOD &= ~T1_MASK;
    TMOD |= T1_M1;

    /* Set ratio for baud rate and oscillator frequency */
    /* SMOD1 SMOD0 - POF GF1 GF0 PD IDL
       1     -     - -   -   -   -  -   */
    PCON |= SMOD;
    TH1 = 256 - tmin;
    /* TL1 = TH1; */

    /* Start Timer1 */
    TR1 = 1;

    return 0;
}

/* Initialize UART */
void uart_init(void)
{
    BUF_INIT(rcbuf);
    BUF_INIT(txbuf);
    
    txoff = 1;
    rcoff = 0;

    /* SM0 SM1 SM2 REN TB8 RB8 TI RI
       0   1   0   1   0   0   0  0  */
    SCON = 0x50;

    /* Enable UART interrupt */
    ES = 1;
    /* Enable global interrupt */
    EA = 1;
}

/* Send a string */
void uart_putstr(const unsigned char __code *s)
{
    for (; *s != 0; s++) {
        uart_putchar(*s);
    }
}

/* Send an unsigned int */
void uart_putuint(unsigned int i)
{
    unsigned char __idata       buf[5];
    unsigned char               j;
    
    uint2bcd(i, buf);
    for (j = 0; j < 4 && buf[j] == 0; j++);
    for (; j < 5; j++) {
        uart_putchar('0' + buf[j]);
    }
}

/* Send an int */
void uart_putint(int i)
{
    if (i < 0) {
        uart_putchar('-');
        i = -i;
    }
    uart_putuint(i);
}

