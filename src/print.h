#ifndef __PRINT_H
#define __PRINT_H


#define PRINT_ZERO      0x01
#define PRINT_PLUS      0x02
#define PRINT_MINUS     0x04
#define PRINT_LEFTALIGN PRINT_MINUS
#define PRINT_SPACE     0x08
#define PRINT_HEX       0x20
#define PRINT_UPPERCASE 0x40
#define PRINT_UNSIGNED  0x80


typedef void (*putchar_func)(unsigned char);


#define UARTCHAR(c)                                             \
    uart_putchar(c)

#define UARTUINT(num)                                           \
    print_int(uart_putchar, PRINT_UNSIGNED, 0, num)

#define UARTINT(num)                                            \
    print_int(uart_putchar, PRINT_UNSIGNED, 0, num)

#define UARTHEX2(num)                                           \
    print_int(uart_putchar,                                     \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              2,                                                \
              num)

#define UARTHEX4(num)                                           \
    print_int(uart_putchar,                                     \
              PRINT_HEX | PRINT_UPPERCASE | PRINT_ZERO,         \
              4,                                                \
              num)

#define UARTSTR(str)                                            \
    print_str(uart_putchar, 0, 0, str)


unsigned char print_int(putchar_func putchar,
                        unsigned char fmt,
                        unsigned char width,
                        int num);
unsigned char print_str(putchar_func putchar, 
                        unsigned char fmt, 
                        unsigned char width, 
                        const unsigned char *str);


#endif /* __PRINT_H */
