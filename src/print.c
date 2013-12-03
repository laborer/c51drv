/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "print.h"


unsigned char print_int(putchar_func putchar,
                        unsigned char fmt,
                        unsigned char width,
                        int num)
{
    unsigned char __idata       buf[5];
    unsigned char __idata       *p;
    unsigned char               sign;
    unsigned char               n;
    unsigned char               ret;

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

    if (fmt & PRINT_HEX) {
        uint2hex(num, buf);
        n = 4;
    } else {
        uint2bcd(num, buf);
        n = 5;
    }

    for (p = buf; *p == 0 && n != 1; p++, n--);

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

    if (!(fmt & (PRINT_LEFTALIGN | PRINT_ZERO)) && width) {
        do {
            putchar(' ');
        } while (--width);
    }

    if (sign) {
        putchar(sign);
    }

    if ((fmt & PRINT_ZERO) && width) {
        do {
            putchar('0');
        } while (--width);
    }

    sign = (fmt & PRINT_UPPERCASE) ? ('A' - 10) : ('a' - 10);
    for (; n != 0; p++, n--) {
        putchar(*p + ((*p > 9) ? sign : '0'));
    }

    if (width) {
        do {
            putchar(' ');
        } while (--width);
    }

    return ret;
}

unsigned char print_str(putchar_func putchar, 
                        unsigned char fmt, 
                        unsigned char width, 
                        const unsigned char *str)
{
    unsigned char       ret;

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
