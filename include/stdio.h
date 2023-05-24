#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include "variables.h"
#endif

void kprint(char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        vidptr[current_loc++] = str[i++];
        vidptr[current_loc++] = color;
    }
}

void kprint_newline(void)
{
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
    current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void)
{
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = color;
    }
}

void inb(unsigned short int port, unsigned char *data)
{
    asm volatile("inb %1, %0" : "=a" (*data) : "d" (port));
}

void outb(unsigned short int port, unsigned char data)
{
    asm volatile("outb %1, %0" : : "d" (port), "a" (data));
}