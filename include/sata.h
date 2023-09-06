//sata read/write functions

#ifndef DMA_INCL
#define DMA_INCL
#include "ahcihdd.h"
#endif

#ifndef STDIO_INCL
#define STDIO_INCL
#include "stdio.h"
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include "string.h"
#endif


void sata_read(unsigned long lba, unsigned long count, unsigned long* buf){
    // read from sata
    // lba: logical block address
    // count: number of blocks to read
    // buf: buffer to read into
    // read from sata into buf

    
}