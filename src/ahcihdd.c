#ifndef STDIO_INCL
#define STDIO_INCL
#include <stdio.h>
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include <string.h>
#endif

#ifndef TASK_INCL
#define TASK_INCL
#include <taskschedule.h>
#endif

#ifndef AHCIHDD_INCL
#include <ahcihdd.h>
#define AHCIHDD_INCL
#endif

void initDrive(){
	FIS_REG_H2D fis;
	ATA_COMMAND command;
	memset(&fis, 0, sizeof(FIS_REG_H2D));
	fis.fis_type = FIS_TYPE_REG_H2D;
	fis.command = ATA_COMMAND_IDENTIFY;	// 0xEC
	fis.device = 0;			// Master device
	fis.c = 1;				// Write command register

	// Send the packet
	for(int i = 0; i < 5; i++){
		outl(0x1F0 + i * 4, ((uint32_t*)&fis)[i]);
	}
}

// variadic function to trace output to the screen
void trace_ahci(char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	va_end(args);

	//print to screen using kprint
	kprint(fmt);
	kprint_newline();
}

HBA_PORT getDriveInfo(){
	
}
