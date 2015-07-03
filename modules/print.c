/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "print.h"


/* Print integer num using function putchar and return the number of
   characters printed.  It provides the similar functionality as
   printf("%d", num). E.g.,
   print_int(uart_putchar, 0, 10, num)
       is same as printf("%10d", num);
   print_int(uart_putchar, PRINT_ZERO, 4, num)
       is same as printf("%04d", num);
   print_int(uart_putchar, PRINT_LEFTALIGN, 6, num)
       is same as printf("%-6d", num);
   print_int(uart_putchar, PRINT_UNSIGNED, 0, num)
       is same as printf("%u", num);
   print_int(uart_putchar, PRINT_HEX | PRINT_UPPERCASE, 0, num)
       is same as printf("%X", num); */
uint8_t print_int(putchar_func putchar,
                  uint8_t fmt,
                  uint8_t width,
                  int16_t num)
{
    uint8_t __idata     buf[5];
    uint8_t __idata     *p;
    uint8_t             sign;
    uint8_t             n;
    uint8_t             ret;

    /* Decide which sign should be used */
    if (num < 0 && !(fmt & (PRINT_UNSIGNED | PRINT_HEX))) {
        num = -num;
        sign = '-';
    } else if (fmt & PRINT_PLUS) {
        sign = '+';
    } else if (fmt & PRINT_SPACE) {
        sign = ' ';
    } else {
        sign = 0;
    }

    /* Convert num to hex or bcd format according to fmt */
    if (fmt & PRINT_HEX) {
        uint2hex(num, buf);
        n = 4;
    } else {
        uint2bcd(num, buf);
        n = 5;
    }

    /* Trim zeros at the front */
    for (p = buf; *p == 0 && n != 1; p++, n--);

    /* Calculate the size of the padding and number of characters to
       print */
    if (width > n) {
        ret = width;
        width -= n;
        if (sign) {
            width -= 1;
        }
    } else {
        ret = n;
        width = 0;
        if (sign) {
            ret += 1;
        }
    }

    /* Normal case, the padding is at the front */
    if (!(fmt & (PRINT_LEFTALIGN | PRINT_ZERO)) && width) {
        do {
            putchar(' ');
        } while (--width);
    }

    /* Print sign if necessary */
    if (sign) {
        putchar(sign);
    }

    /* Zero padding case (e.g. "%04d"), the padding is in between the sign
       and number */
    if ((fmt & PRINT_ZERO) && width) {
        do {
            putchar('0');
        } while (--width);
    }

    /* Print the number */
    sign = (fmt & PRINT_UPPERCASE) ? ('A' - 10) : ('a' - 10);
    for (; n != 0; p++, n--) {
        putchar(*p + ((*p > 9) ? sign : '0'));
    }

    /* Left alignment case, the padding is at the end */
    if (width) {
        do {
            putchar(' ');
        } while (--width);
    }

    return ret;
}

/* Print string str using function putchar and return the number of
   characters printed.  It provides the similar functionality as
   printf("%s", num). E.g.,
   print_int(uart_putchar, 0, 10, str)
       is same as printf("%10s", str); */
uint8_t print_str(putchar_func putchar,
                  uint8_t fmt,
                  uint8_t width,
                  const uint8_t *str)
{
    uint8_t       ret;

    for (ret = 0; *str != 0; str++, ret++);
    str -= ret;

    if (width > ret) {
        width -= ret;
        ret += width;
    } else {
        width = 0;
    }

    if (!(fmt & PRINT_LEFTALIGN) && width) {
        do {
            putchar(' ');
        } while (--width);
    }

    for (; *str != 0; str++) {
        putchar(*str);
    }

    if (width) {
        do {
            putchar(' ');
        } while (--width);

    }

    return ret;
}
