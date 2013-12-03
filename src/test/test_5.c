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
#include "print.h"


#define PUTCHAR(c)                                              \
    do {                                                        \
        lcd1602_putchar(c);                                     \
    } while (0)

#define PUTSTR(str)                                             \
    do {                                                        \
        print_str(lcd1602_putchar, 0, 0, str);                  \
    } while (0)

#define PUTUINT(num)                                            \
    do {                                                        \
        print_int(lcd1602_putchar, PRINT_UNSIGNED, 0, num);     \
    } while (0)

#define PUTHEXINT(num)                                          \
    do {                                                        \
        print_int(lcd1602_putchar, PRINT_HEX, 0, num);          \
    } while (0)


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
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
    PUTSTR("TEMP: ");

    lcd1602_position(0, 1);
    PUTSTR("CODE: ");

}

static void display_temp_clear()
{
    lcd1602_position(6, 0);
    PUTSTR("        ");
}

static void display_temp(int tempcode)
{
    unsigned int k;

    lcd1602_position(6, 0);
    if (tempcode < 0) {
        tempcode = -tempcode;
        PUTCHAR('-');
    }
    
    PUTUINT(tempcode >> 4);
    PUTCHAR('.');
    
    k = (unsigned char)(((unsigned char)tempcode & 0x0F) * 10 + 8) >> 4;
    
    PUTCHAR('0' + k);
    PUTCHAR('\xDF');
    PUTCHAR('C');
}

static void display_ir_clear()
{
    lcd1602_position(6, 1);
    PUTSTR("    ");
}

static void display_ir(unsigned int ircode)
{
    lcd1602_position(6, 1);
    PUTHEXINT(ircode >> 12);
    PUTHEXINT(ircode >> 8);
    PUTHEXINT(ircode >> 4);
    PUTHEXINT(ircode);
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
            }
        }

        if (irstate == 1) {
            irstate = 2;
            display_ir(ircode);
            irtime = TIMER0_GET32();
        } else if (irstate == 2) {
            if (TIMER0_GET32() - irtime > 2000UL * TIMER_CYCLES_US(1000)) {
                irstate = 0;
                display_ir_clear();
            }
        }
        
        PCON |= IDL;
    }
}
