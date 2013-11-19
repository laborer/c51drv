/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "stc/stc.h"
#include "stc/pca.h"


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    welcome();

    uart_getchar();

    PCA_INIT(PCA_SYSCLK_8);

    PCA_PWMOUT0(128);

    PCA_PWMOUT1(192);

    while (1);
}
