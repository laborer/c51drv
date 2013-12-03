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


#define UARTCHAR(c)                                              \
    do {                                                        \
        uart_putchar(c);                                        \
    } while (0)

#define UARTUINT(num)                                            \
    do {                                                        \
        print_int(uart_putchar, PRINT_UNSIGNED, 0, num);        \
    } while (0)

#define UARTINT(num)                                             \
    do {                                                        \
        print_int(uart_putchar, PRINT_UNSIGNED, 0, num);        \
    } while (0)

#define UARTHEX2(num)                                            \
    do {                                                        \
        print_int(uart_putchar,                                 \
                  PRINT_HEX | PRINT_ZERO, 2, num);              \
    } while (0)

#define UARTHEX4(num)                                            \
    do {                                                        \
        print_int(uart_putchar,                                 \
                  PRINT_HEX | PRINT_ZERO, 4, num);              \
    } while (0)

#define UARTSTR(str)                                             \
    do {                                                        \
        print_str(uart_putchar, 0, 0, str);                     \
    } while (0)


unsigned char print_int(putchar_func putchar,
                        unsigned char fmt,
                        unsigned char width,
                        int num);
unsigned char print_str(putchar_func putchar, 
                        unsigned char fmt, 
                        unsigned char width, 
                        const unsigned char *str);


#endif /* __PRINT_H */
