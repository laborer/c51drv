#ifndef __PRINT_H
#define __PRINT_H


#include "common.h"


/* Zero padding.  Similar to printf("%04d", ..) */
#define PRINT_ZERO      0x01
/* Use plus sign for positive number.  Similar to printf("%+d", ..) */
#define PRINT_PLUS      0x02
/* Left alignment.  Similar to printf("%-d", ..) */
#define PRINT_MINUS     0x04
#define PRINT_LEFTALIGN PRINT_MINUS
/* Space before a positive number.  Similar to printf("% d", ..) */
#define PRINT_SPACE     0x08
/* Print hexadecimal.  Similar to printf("%x", ..) */
#define PRINT_HEX       0x20
/* Use uppercase letter.  Similar to printf("%X", ..) */
#define PRINT_UPPERCASE 0x40
/* Print an unsigned int.  Similar to printf("%u", ..) */
#define PRINT_UNSIGNED  0x80


typedef void (*putchar_func)(uint8_t);


/* Print a char using UART */
#define UARTCHAR(c)                                             \
    uart_putchar(c)

/* Print an unsigned int using UART */
#define UARTUINT(num)                                           \
    print_int(uart_putchar, PRINT_UNSIGNED, 0, num)

/* Print an int using UART */
#define UARTINT(num)                                            \
    print_int(uart_putchar, 0, 0, num)

/* Print num as 2-digit hexadecimal using UART */
#define UARTHEX2(num)                                           \
    print_int(uart_putchar,                                     \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              2,                                                \
              (uint8_t)num)

/* Print num as 4-digit hexadecimal using UART */
#define UARTHEX4(num)                                           \
    print_int(uart_putchar,                                     \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              4,                                                \
              num)

/* Print string str using UART */
#define UARTSTR(str)                                            \
    print_str(uart_putchar, 0, 0, str)


uint8_t print_int(putchar_func putchar,
                  uint8_t fmt,
                  uint8_t width,
                  int16_t num);
uint8_t print_str(putchar_func putchar,
                  uint8_t fmt,
                  uint8_t width,
                  const uint8_t *str);


#endif /* __PRINT_H */
