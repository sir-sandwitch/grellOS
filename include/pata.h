#ifndef STDIO_INCL
#define STDIO_INCL
#include "stdio.h"
#endif

#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include "variables.h"
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include "string.h"
#endif

/*ATA (PATA) Driver*/
/*This driver is for PATA drives only. It is not compatible with SATA drives.*/
/*This driver is not compatible with ATAPI devices.*/

void ata_init();
void ata_read(unsigned int sector, unsigned char* buffer);
void ata_write(unsigned int sector, unsigned char* buffer);
void ata_flush();
void ata_read_sector(unsigned int sector, unsigned char* buffer);
void ata_write_sector(unsigned int sector, unsigned char* buffer);
void ata_flush_sector();
void ata_read_block(unsigned int sector, unsigned char* buffer);
void ata_write_block(unsigned int sector, unsigned char* buffer);
void ata_flush_block();
