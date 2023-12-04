#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include <variables.h>
#endif

#ifndef STDIO_INCL
#define STDIO_INCL
#include <stdio.h>
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

char inb(unsigned short int port)
{
    unsigned char data;
    asm volatile("inb %1, %0" : "=a" (data) : "d" (port));
    return data;
}

void outb(unsigned short int port, unsigned char data)
{
    asm volatile("outb %1, %0" : : "d" (port), "a" (data));
}

void outl(unsigned short int port, unsigned long data)
{
    asm volatile("outl %1, %0" : : "d" (port), "a" (data));
}

unsigned long inl(unsigned short int port)
{
    unsigned long data;
    asm volatile("inl %1, %0" : "=a" (data) : "d" (port));
    return data;
}

void memset(void* ptr, int value, unsigned int num)
{
    unsigned char* ptr2 = (unsigned char*)ptr;
    while (num-- > 0) {
        *ptr2++ = value;
    }
}