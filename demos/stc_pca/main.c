/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <stc/stc.h>
#include <stc/pca.h>


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    welcome();

    uart_getchar();

    PCA_INIT(PCA_SYSCLK_8);

    PCA0_PWMOUT(128);

    PCA1_PWMOUT(192);

    while (1);
}
