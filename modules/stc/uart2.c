/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "../common.h"
#include "../tools.h"
#include "stc.h"
#include "uart2.h"


#define S2RI    0x01
#define S2TI    0x02


/* Ring buffer struct for UART2 I/O */
typedef struct {
    uint8_t     dat[4];
    uint8_t     in;
    uint8_t     out;
} buffer_t;


/* Transmitting buffer */
static buffer_t txbuf;
/* Receiving buffer */
static buffer_t rcbuf;

/* Transmitting is turned off */
static int8_t   txoff;
/* Receiving buffer is overflow */
static int8_t   rcoff;


#ifdef UART2_CALLBACK
extern void UART2_CALLBACK(uint8_t c) __using 1;
#endif /* UART2_CALLBACK */

/* UART2 interrupt routine */
void uart2_interrupt(void) __interrupt 8 __using 1
{
    uint8_t c;

    /* Check if UART2 is ready to read */
    if (S2CON & S2RI) {
        /* Reset receiving interrupt flag */
        S2CON &= ~S2RI;
        /* Read one byte from S2BUF */
        c = S2BUF;
        /* Check if receiving buffer is full */
        if (BUF_FULL(rcbuf)) {
            /* Set buffer overflow flag */
            rcoff = 1;
        } else {
            /* Read one byte from SBUF and put it in receiving
               buffer */
            BUF_PUT(rcbuf, c);
        }
#ifdef UART2_CALLBACK
        UART2_CALLBACK(c);
#endif /* UART2_CALLBACK */
    }

    /* Check if UART2 is ready to write */
    if (S2CON & S2TI) {
        /* Reset transmitting interrupt flag */
        S2CON &= ~S2TI;
        /* Check if transmitting buffer is empty */
        if (BUF_EMPTY(txbuf)) {
            /* Turn off transmitting */
            txoff = 1;
        } else {
            /* Read a byte from transmitting buffer and put it in
               S2BUF */
            S2BUF = BUF_GET(txbuf);
        }
    }
}

/* Test if there are data ready to be read */
int8_t uart2_rcready(void) {
    return !BUF_EMPTY(rcbuf);
}

/* Test if it is ready to send data */
int8_t uart2_txready(void) {
    return !BUF_FULL(txbuf);
}

/* Send a byte in block mode */
void uart2_putchar(uint8_t c)
{
    while (BUF_FULL(txbuf));

    BUF_PUT(txbuf, c);
    /* Check if transmitting is turned off */
    if (txoff) {
        /* Turn on transmitting */
        txoff = 0;
        /* Set transmitting interrupt flag to be ready */
        S2CON |= S2TI;
    }
}

/* Read a byte in block mode */
uint8_t uart2_getchar(void)
{
    while (BUF_EMPTY(rcbuf));

    return BUF_GET(rcbuf);
}

/* Set a fixed baudrate */
void uart2_baudrate(void)
{
    /* Set ratio for baud rate and oscillator frequency */
    /* T0x12 T1x12 UART_M0x6 BRTR S2SMOD BRTx12 EXTRAM S1BRS
       -     -     -         1    x      x      -      -     */
    AUXR &= ~0x0C;
    AUXR |= 0x10 | (UART2_SMOD ? 0x08 : 0) | (UART2_BRTX12 ? 0x04 : 0);

    /* Set timer */
    /* 256 - FOSC * (SMOD1 + 1) / BAUD / 32 / 12 */
    BRT = -(FOSC / UART2_BAUD / (UART2_SMOD ? 16 : 32)
            / (UART2_BRTX12 ? 1 : 12));
}

/* Initialize UART2 */
void uart2_init(void)
{
    BUF_INIT(rcbuf);
    BUF_INIT(txbuf);

    txoff = 1;
    rcoff = 0;

    /* Use Timer1 as baudrate generator */
    /* S2SM0 S2SM1 S2SM2 S2REN S2TB8 S2RB8 S2TI S2RI
       0     1     0     1     0     0     0    0  */
    S2CON = 0x50;

    /* Enable UART2 interrupt */
    /* - - - - - - ESPI ES2
       - - - - - - -    1   */
    IE2 |= 0x01;
    /* Enable global interrupt */
    EA = 1;
}
