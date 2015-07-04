/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <irnec.h>
#include <lcd1602.h>
#include <rom9346.h>
#include <ds1820.h>
#include <timer.h>
#include <print.h>


#define LCDCHAR(c)                                              \
    lcd1602_putchar(c);

#define LCDSTR(str)                                             \
    print_str(lcd1602_putchar, 0, 0, str)

#define LCDINT4(num)                                            \
    print_int(lcd1602_putchar, 0, 4, num)

#define LCDUINT(num)                                            \
    print_int(lcd1602_putchar, PRINT_UNSIGNED, 0, num)

#define LCDHEX4(num)                                            \
    print_int(lcd1602_putchar,                                  \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              4,                                                \
              num)


static volatile char irstate;
static unsigned int ircode;


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void _irnec_int0(void) __interrupt IE0_VECTOR __using 1
{
    if (irnec_falling() == 0) {
        ircode = irnec_result();
        irstate = 1;
    }
}

static void display_init(void)
{
    lcd1602_init();
}

static void display_temp_clear(void)
{
    LCD1602_POSITION(0, 0);
    LCDSTR("        ");
}

static void display_temp(int tempcode)
{
    unsigned int k;

    LCD1602_POSITION(0, 0);
    LCDINT4(tempcode >> 4);
    LCDCHAR('.');

    k = tempcode & 0x0F;
    if (tempcode < 0) {
        k = 0x10 - k;
    }
    k = (unsigned char)(k * 10 + 8) >> 4;

    LCDCHAR('0' + k);
    LCDCHAR('\xDF');
    LCDCHAR('C');
}

static void display_ir_clear(void)
{
    LCD1602_POSITION(10, 0);
    LCDSTR("    ");
}

static void display_ir(unsigned int ircode)
{
    LCD1602_POSITION(10, 0);
    LCDHEX4(ircode);
}

static void display_rom(void)
{
    static unsigned char pos;
    unsigned char i;
    unsigned char c;

    for (i = 0; i < 18; i++) {
        LCD1602_POSITION(i + 1, 1);
        c = lcd1602_getchar();
        LCD1602_POSITION(i, 1);
        LCDCHAR(c);
    }

    /* pos = (pos + 1) & (64 * 4 - 1); */
    pos += 1;
    if (!(pos & 0x03)) {
        LCD1602_POSITION(15, 1);
        LCDHEX4(rom9346_read(pos >> 2));
    }
}

void main(void) {
    int                 tempstate;
    int                 tempcode;
    unsigned long       temptime;
    unsigned long       irtime;
    unsigned char       romstate;

    welcome();

    display_init();

    irnec_init(); 
    /* INT0 */
    P3_2 = 1;
    irstate = 0;
    IT0 = 1;
    EX0 = 1;
    EA = 1;

    temptime = 0;
    irtime = 0;
    tempstate = 0;
    romstate = 0;

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

        if (romstate == 0) {
            display_rom();
        }
        romstate = (romstate + 1) & 0x03;

        POWER_IDLE();
    }
}
