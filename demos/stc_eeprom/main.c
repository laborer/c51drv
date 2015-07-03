/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <stc/eeprom.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned int i;

    welcome();

    /* for (i = 0; i < 128; i++) { */
    /*     eeprom_write(i + 0x2000 + 256 + 192, i); */
    /* } */

    eeprom_erase(0x2000);

    for (i = 0; i < 256; i++) {
        UARTCHAR(eeprom_read(i + 0x2000 + 256 + 128));
    }

    while (1);
}


