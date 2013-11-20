/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "irnec.h"
#include "lcd1602.h"
#include "rom9346.h"
#include "ds1820.h"
#include "timer.h"


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

static void _lcd1602_putstr(const unsigned char __code *s)
{
    for (; *s != 0; s++) {
        lcd1602_putchar(*s);
    }
}

static void _lcd1602_putuint(unsigned int i)
{
    unsigned char __idata       buf[5];
    unsigned char               j;
    
    uint2bcd(i, buf);
    for (j = 0; j < 4 && buf[j] == 0; j++);
    for (; j < 5; j++) {
        lcd1602_putchar('0' + buf[j]);
    }
}

static void _lcd1602_puthex(char c)
{
    c &= 0x0F;
    lcd1602_putchar((c > 9) ? ('A' - 10 + c) : ('0' + c));
}       

static volatile char irstate;
static unsigned int ircode;

void _irnec_int0(void) __interrupt IE0_VECTOR __using 1
{
    if (irnec_falling() == 0) {
        ircode = irnec_result();
        irstate = 1;
    }
}

static void display_init()
{
    lcd1602_init();

    lcd1602_position(0, 0);
    _lcd1602_putstr("TEMP: ");

    lcd1602_position(0, 1);
    _lcd1602_putstr("CODE: ");

}

static void display_temp_clear()
{
    lcd1602_position(6, 0);
    _lcd1602_putstr("        ");
}

static void display_temp(int tempcode)
{
    unsigned int k;

    lcd1602_position(6, 0);
    if (tempcode < 0) {
        tempcode = -tempcode;
        lcd1602_putchar('-');
    }
    
    _lcd1602_putuint(tempcode >> 4);
    lcd1602_putchar('.');
    
    k = (unsigned char)(((unsigned char)tempcode & 0x0F) * 10 + 8) >> 4;
    
    lcd1602_putchar('0' + k);
    lcd1602_putchar('\xDF');
    lcd1602_putchar('C');
}

static void display_ir_clear()
{
    lcd1602_position(6, 1);
    _lcd1602_putstr("    ");
}

static void display_ir(unsigned int ircode)
{
    lcd1602_position(6, 1);
    _lcd1602_puthex(ircode >> 12);
    _lcd1602_puthex(ircode >> 8);
    _lcd1602_puthex(ircode >> 4);
    _lcd1602_puthex(ircode);
}

void main(void) {
    int                 tempstate;
    int                 tempcode;
    unsigned long       temptime;
    unsigned long       irtime;

    welcome();

    display_init();

    irnec_init(); 
    P3_2 = 1;
    irstate = 0;
    IT0 = 1;
    EX0 = 1;
    EA = 1;

    tempstate = 0;

    TIMER0_INIT32();
    TIMER0_START();

    while (1) {
        if (tempstate == 0) {
            if (ds1820_convert_t(0) == 0) {
                temptime = TIMER0_GET32();
                tempstate = 1;
            }
        } else if (tempstate == 1) {
            if (TIMER0_GET32() - temptime > 800UL * TIMER_CYCLES_US(1000)) {
                tempcode = ds1820_read_temperature(0);
                if (((unsigned int)tempcode >> 8) != 0x80) {
                    display_temp(tempcode);
                } else {
                    display_temp_clear();
                }
                tempstate = 0;
                /* uart_putchar('T'); */
            }
        }

        if (irstate == 1) {
            irstate = 2;
            display_ir(ircode);
            irtime = TIMER0_GET32();
            /* uart_putchar('I'); */
        } else if (irstate == 2) {
            if (TIMER0_GET32() - irtime > 2000UL * TIMER_CYCLES_US(1000)) {
                irstate = 0;
                display_ir_clear();
            }
        }
        
        PCON |= IDL;
    }
}
