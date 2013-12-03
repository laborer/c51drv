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


unsigned char print_int(putchar_func putchar,
                        unsigned char fmt,
                        unsigned char width,
                        int num);
/* unsigned char print_uchar(putchar_func putchar,  */
/*                           unsigned char fmt,  */
/*                           unsigned char width,  */
/*                           unsigned char num); */
/* unsigned char print_char(putchar_func putchar,  */
/*                          unsigned char fmt,  */
/*                          unsigned char width,  */
/*                          char num); */
/* unsigned char print_hexchar(putchar_func putchar,  */
/*                             unsigned char fmt,  */
/*                             unsigned char width,  */
/*                             unsigned char num); */
/* unsigned char print_uint(putchar_func putchar,  */
/*                          unsigned char fmt,  */
/*                          unsigned char width,  */
/*                          unsigned int num); */
/* unsigned char print_int(putchar_func putchar,  */
/*                         unsigned char fmt,  */
/*                         unsigned char width,  */
/*                         int num); */
/* unsigned char print_hexint(putchar_func putchar,  */
/*                            unsigned char fmt,  */
/*                            unsigned char width,  */
/*                            unsigned int num); */
/* unsigned char print_ulong(//putchar_func putchar,  */
/*                           unsigned char fmt,  */
/*                           unsigned char width,  */
/*                           unsigned long num); */
/* unsigned char print_long(putchar_func putchar,  */
/*                          unsigned char fmt,  */
/*                          unsigned char width,  */
/*                          long num); */
/* unsigned char print_hexlong(putchar_func putchar,  */
/*                             unsigned char fmt,  */
/*                             unsigned char width,  */
/*                             unsigned long num); */
unsigned char print_str(putchar_func putchar, 
                        unsigned char fmt, 
                        unsigned char width, 
                        const unsigned char *str);


#endif /* __PRINT_H */
