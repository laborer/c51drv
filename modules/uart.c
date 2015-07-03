/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "uart.h"
#include "timer.h"


/* Ring buffer struct for UART I/O */
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
static __bit txoff;
/* Receiving buffer is overflow */
static __bit rcoff;


#ifdef UART_CALLBACK
extern void UART_CALLBACK(uint8_t c) __using 1;
#endif /* UART_CALLBACK */

/* UART interrupt routine */
void uart_interrupt(void) __interrupt SI0_VECTOR __using 1
{
    uint8_t c;

    /* Check if UART is ready to read */
    if (RI) {
        /* Reset receiving interrupt flag */
        RI = 0;
        /* Read one byte from SBUF */
        c = SBUF;
        /* Check if receiving buffer is full */
        if (BUF_FULL(rcbuf)) {
            /* Set buffer overflow flag */
            rcoff = 1;
        } else {
            /* Read one byte from SBUF and put it in receiving
               buffer */
            BUF_PUT(rcbuf, c);
        }
#ifdef UART_CALLBACK
        UART_CALLBACK(c);
#endif /* UART_CALLBACK */
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

/* Test if there are data ready to be read */
int8_t uart_rcready(void) {
    return !BUF_EMPTY(rcbuf);
}

/* Test if it is ready to send data */
int8_t uart_txready(void) {
    return !BUF_FULL(txbuf);
}

/* Send a byte in block mode */
void uart_putchar(uint8_t c)
{
    while (BUF_FULL(txbuf));

    BUF_PUT(txbuf, c);
    /* Check if transmitting is turned off */
    if (txoff) {
        /* Turn on transmitting */
        txoff = 0;
        /* Set transmitting interrupt flag to be ready */
        TI = 1;
    }
}

/* Read a byte in block mode */
uint8_t uart_getchar(void)
{
    while (BUF_EMPTY(rcbuf));

    return BUF_GET(rcbuf);
}

/* Set a fixed baudrate */
void uart_baudrate(void)
{
    /* Set ratio for baud rate and oscillator frequency */
    if (UART_SMOD) {
        /* SMOD1 SMOD0 - POF GF1 GF0 PD IDL
           1     -     - -   -   -   -  -   */
        PCON |= SMOD;
    } else {
        PCON &= ~SMOD;
    }

    /* Set timer */
    /* 256 - FOSC * (SMOD1 + 1) / BAUD / 32 / 12 */ 
    TIMER1_INIT8(-(FOSC / UART_BAUD / (UART_SMOD ? 16 : 32) / 12));
    TIMER1_START();
}

/* Detect and set baudrate automatically  */
int8_t uart_baudrate_auto(void)
{
    uint16_t    t;
    uint16_t    tmax;
    uint16_t    tmin;
    uint8_t     i;
    uint8_t     k;

    /* Disable Timer1 interrupt */
    ET1 = 0;
    /* Disable UART interrupt */
    ES = 0;

    /* Set Timer1 to be a 16-bit timer */
    TIMER1_INIT16();

    tmax = 0;
    tmin = 0xFFFF;
    /* k stores the number of successfully captured low level
       interval */
    k = 0;

    for (i = 0; i < 200; i++) {
        /* Wait RXD to be high */
        while (!RXD);
        /* Reset Timer1 */
        TIMER1_SET16(0);
        /* Reset Timer1 overflow flag */
        TIMER1_FLAG = 0;
        /* Resume Timer1 */
        TIMER1_START();
        /* Wait for a falling edge of RXD before Timer1 overflows */
        while (RXD && !TIMER1_FLAG);
        /* Reset Timer1 */
        TIMER1_SET16(0);
        /* Restart if it waited for too long */
        if (TIMER1_FLAG) {
            continue;
        }
        /* Wait for a rising edge of RXD */
        while (!RXD);
        /* Pause Timer1 */
        TIMER1_STOP();
        /* The baudrate is too low if Timer1 overflows */
        if (TIMER1_FLAG) {
            return -1;
        }

        /* Now we captured one low level interval */
        k += 1;

        /* Assume SMOD1=1 */
        /* t = (TH1 * 256 + TL1 + 8) / 16; */
        t = (TIMER1_GET16() + 8) >> 4;
        if (t > tmax) {
            tmax = t;
        }
        if (t < tmin) {
            tmin = t;
        }
        if (tmax >= tmin * 5) {
            k = 0xFF;
            break;
        }
    }

    if (tmin >> 8) {
        return -128;
    }
    if (k < 3) {
        return -2;
    }
    if (tmax * 2 < tmin * 5) {
        return -1;
    }

    /* Set ratio for baud rate and oscillator frequency */
    /* SMOD1 SMOD0 - POF GF1 GF0 PD IDL
       1     -     - -   -   -   -  -   */
    PCON |= SMOD;

    /* Set timer */
    /* 256 - FOSC * (SMOD1 + 1) / BAUD / 32 / 12 */ 
    TIMER1_INIT8(256 - tmin);
    TIMER1_START();

    return 0;
}

/* Initialize UART */
void uart_init(void)
{
    BUF_INIT(rcbuf);
    BUF_INIT(txbuf);
    
    txoff = 1;
    rcoff = 0;

    /* Use Timer1 as baudrate generator */
    /* SM0 SM1 SM2 REN TB8 RB8 TI RI
       0   1   0   1   0   0   0  0  */
    SCON = 0x50;

    /* Enable UART interrupt */
    ES = 1;
    /* Enable global interrupt */
    EA = 1;
}
