#ifndef STDIO_INCL
#define STDIO_INCL
#include "stdio.h"
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include "string.h"
#endif

#ifndef TASK_INCL
#define TASK_INCL
#include "taskschedule.h"
#endif

#include <stdarg.h>

// ahci SATA drive support

typedef enum
{
	ATA_COMMAND_IDENTIFY		= 0xEC,
	ATA_COMMAND_READ_DMA_EXT	= 0x25,
	ATA_COMMAND_WRITE_DMA_EXT	= 0x35,
	ATA_COMMAND_READ_DMA		= 0xC8,
	ATA_COMMAND_WRITE_DMA		= 0xCA,
	ATA_COMMAND_READ_SECTORS	= 0x20,
	ATA_COMMAND_WRITE_SECTORS	= 0x30,
	ATA_COMMAND_READ_SECTORS_EXT	= 0x24,
	ATA_COMMAND_WRITE_SECTORS_EXT	= 0x34,
	ATA_COMMAND_FLUSH_CACHE		= 0xE7,
	ATA_COMMAND_FLUSH_CACHE_EXT	= 0xEA,
	ATA_COMMAND_PACKET		= 0xA0,
	ATA_COMMAND_IDENTIFY_PACKET	= 0xA1,
	ATA_COMMAND_READ_MULTIPLE	= 0xC4,
	ATA_COMMAND_WRITE_MULTIPLE	= 0xC5,
	ATA_COMMAND_SET_MULTIPLE	= 0xC6,
	ATA_COMMAND_READ_MULTIPLE_EXT	= 0x29,
	ATA_COMMAND_WRITE_MULTIPLE_EXT	= 0x39,
	ATA_COMMAND_READ_LOG_EXT	= 0x2F,
	ATA_COMMAND_WRITE_LOG_EXT	= 0x3F,
	ATA_COMMAND_READ_FPDMA		= 0x60,
	ATA_COMMAND_WRITE_FPDMA		= 0x61,
	ATA_COMMAND_NCQ_NON_DATA	= 0x63,
	ATA_COMMAND_SEND_FPDMA		= 0x64,
	ATA_COMMAND_RECEIVE_FPDMA	= 0x65,
	ATA_COMMAND_DOWNLOAD_MICROCODE	= 0x92,
	ATA_COMMAND_STANDBY_IMMEDIATE	= 0xE0,
	ATA_COMMAND_IDLE_IMMEDIATE	= 0xE1,
	ATA_COMMAND_STANDBY		= 0xE2,
	ATA_COMMAND_IDLE		= 0xE3,
	ATA_COMMAND_READ_BUFFER		= 0xE4,
	ATA_COMMAND_CHECK_POWER		= 0xE5,
	ATA_COMMAND_SLEEP		= 0xE6,
	ATA_COMMAND_WRITE_BUFFER	= 0xE8,
	ATA_COMMAND_WRITE_SAME		= 0xE9,
	ATA_COMMAND_SECURITY_SET_PASSWORD	= 0xF1,
	ATA_COMMAND_SECURITY_UNLOCK	= 0xF2,
	ATA_COMMAND_SECURITY_ERASE_PREPARE	= 0xF3,
	ATA_COMMAND_SECURITY_ERASE_UNIT	= 0xF4,
	ATA_COMMAND_SECURITY_FREEZE_LOCK	= 0xF5,
	ATA_COMMAND_SECURITY_DISABLE	= 0xF6,
	ATA_COMMAND_READ_NATIVE_MAX_ADDRESS	= 0xF8,
	ATA_COMMAND_SET_MAX_ADDRESS	= 0xF9,
	ATA_COMMAND_READ_VERIFY_SECTORS	= 0x40,
	ATA_COMMAND_READ_VERIFY_SECTORS_EXT	= 0x42,
	ATA_COMMAND_READ_VERIFY		= 0x41,
	ATA_COMMAND_READ_VERIFY_EXT	= 0x42,
	ATA_COMMAND_WRITE_UNCORRECTABLE_EXT	= 0x45,
	ATA_COMMAND_WRITE_UNCORRECTABLE	= 0x45,
	ATA_COMMAND_READ_DMA_QUEUED	= 0xC7,
	ATA_COMMAND_WRITE_DMA_QUEUED	= 0xC8,
	ATA_COMMAND_WRITE_DMA_QUEUED_FUA_EXT	= 0x3D,
	ATA_COMMAND_READ_LOG_DMA_EXT	= 0x47,
	ATA_COMMAND_WRITE_LOG_DMA_EXT	= 0x57,
	ATA_COMMAND_TRUSTED_RECEIVE	= 0x5C,
	ATA_COMMAND_TRUSTED_RECEIVE_DMA	= 0x5D,
	ATA_COMMAND_TRUSTED_SEND	= 0x5E,
	ATA_COMMAND_TRUSTED_SEND_DMA	= 0x5F,
} ATA_COMMAND;

typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;

typedef struct tagFIS_REG_H2D
{
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_REG_H2D
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:3;		// Reserved
	uint8_t  c:1;		// 1: Command, 0: Control
 
	uint8_t  command;	// Command register
	uint8_t  featurel;	// Feature register, 7:0
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  featureh;	// Feature register, 15:8
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  icc;		// Isochronous command completion
	uint8_t  control;	// Control register
 
	// DWORD 4
	uint8_t  rsv1[4];	// Reserved
} FIS_REG_H2D;

typedef struct tagFIS_REG_D2H
{
	// DWORD 0
	uint8_t  fis_type;    // FIS_TYPE_REG_D2H
 
	uint8_t  pmport:4;    // Port multiplier
	uint8_t  rsv0:2;      // Reserved
	uint8_t  i:1;         // Interrupt bit
	uint8_t  rsv1:1;      // Reserved
 
	uint8_t  status;      // Status register
	uint8_t  error;       // Error register
 
	// DWORD 1
	uint8_t  lba0;        // LBA low register, 7:0
	uint8_t  lba1;        // LBA mid register, 15:8
	uint8_t  lba2;        // LBA high register, 23:16
	uint8_t  device;      // Device register
 
	// DWORD 2
	uint8_t  lba3;        // LBA register, 31:24
	uint8_t  lba4;        // LBA register, 39:32
	uint8_t  lba5;        // LBA register, 47:40
	uint8_t  rsv2;        // Reserved
 
	// DWORD 3
	uint8_t  countl;      // Count register, 7:0
	uint8_t  counth;      // Count register, 15:8
	uint8_t  rsv3[2];     // Reserved
 
	// DWORD 4
	uint8_t  rsv4[4];     // Reserved
} FIS_REG_D2H;

typedef struct tagFIS_DATA
{
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_DATA
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:4;		// Reserved
 
	uint8_t  rsv1[2];	// Reserved
 
	// DWORD 1 ~ N
	uint32_t data[1];	// Payload
} FIS_DATA;

typedef struct tagFIS_PIO_SETUP
{
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_PIO_SETUP
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:1;		// Reserved
	uint8_t  d:1;		// Data transfer direction, 1 - device to host
	uint8_t  i:1;		// Interrupt bit
	uint8_t  rsv1:1;
 
	uint8_t  status;		// Status register
	uint8_t  error;		// Error register
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  rsv2;		// Reserved
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  rsv3;		// Reserved
	uint8_t  e_status;	// New value of status register
 
	// DWORD 4
	uint16_t tc;		// Transfer count
	uint8_t  rsv4[2];	// Reserved
} FIS_PIO_SETUP;

typedef struct tagFIS_DMA_SETUP
{
	// DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_DMA_SETUP
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  rsv0:1;		// Reserved
	uint8_t  d:1;		// Data transfer direction, 1 - device to host
	uint8_t  i:1;		// Interrupt bit
	uint8_t  a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed
 
        uint8_t  rsved[2];       // Reserved
 
	//DWORD 1&2
 
        uint64_t DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
                                 // SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
 
        //DWORD 3
        uint32_t rsvd;           //More reserved
 
        //DWORD 4
        uint32_t DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0
 
        //DWORD 5
        uint32_t TransferCount;  //Number of bytes to transfer. Bit 0 must be 0
 
        //DWORD 6
        uint32_t resvd;          //Reserved
 
} FIS_DMA_SETUP;

typedef enum tagFIS_DEV_BITS
{
	FIS_DEV_BITS_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_DEV_BITS_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_DEV_BITS;

typedef volatile struct tagHBA_PORT
{
	uint32_t clb;		// 0x00, command list base address, 1K-byte aligned
	uint32_t clbu;		// 0x04, command list base address upper 32 bits
	uint32_t fb;		// 0x08, FIS base address, 256-byte aligned
	uint32_t fbu;		// 0x0C, FIS base address upper 32 bits
	uint32_t is;		// 0x10, interrupt status
	uint32_t ie;		// 0x14, interrupt enable
	uint32_t cmd;		// 0x18, command and status
	uint32_t rsv0;		// 0x1C, Reserved
	uint32_t tfd;		// 0x20, task file data
	uint32_t sig;		// 0x24, signature
	uint32_t ssts;		// 0x28, SATA status (SCR0:SStatus)
	uint32_t sctl;		// 0x2C, SATA control (SCR2:SControl)
	uint32_t serr;		// 0x30, SATA error (SCR1:SError)
	uint32_t sact;		// 0x34, SATA active (SCR3:SActive)
	uint32_t ci;		// 0x38, command issue
	uint32_t sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	uint32_t fbs;		// 0x40, FIS-based switch control
	uint32_t rsv1[11];	// 0x44 ~ 0x6F, Reserved
	uint32_t vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct tagHBA_MEM
{
	// 0x00 - 0x2B, Generic Host Control
	uint32_t cap;		// 0x00, Host capability
	uint32_t ghc;		// 0x04, Global host control
	uint32_t is;		// 0x08, Interrupt status
	uint32_t pi;		// 0x0C, Port implemented
	uint32_t vs;		// 0x10, Version
	uint32_t ccc_ctl;	// 0x14, Command completion coalescing control
	uint32_t ccc_pts;	// 0x18, Command completion coalescing ports
	uint32_t em_loc;		// 0x1C, Enclosure management location
	uint32_t em_ctl;		// 0x20, Enclosure management control
	uint32_t cap2;		// 0x24, Host capabilities extended
	uint32_t bohc;		// 0x28, BIOS/OS handoff control and status
 
	// 0x2C - 0x9F, Reserved
	uint8_t  rsv[0xA0-0x2C];
 
	// 0xA0 - 0xFF, Vendor specific registers
	uint8_t  vendor[0x100-0xA0];
 
	// 0x100 - 0x10FF, Port control registers
	HBA_PORT	ports[1];	// 1 ~ 32
} HBA_MEM;

typedef volatile struct tagHBA_FIS
{
	// 0x00
	FIS_DMA_SETUP	dsfis;		// DMA Setup FIS
	uint8_t         pad0[4];
 
	// 0x20
	FIS_PIO_SETUP	psfis;		// PIO Setup FIS
	uint8_t         pad1[12];
 
	// 0x40
	FIS_REG_D2H	rfis;		// Register – Device to Host FIS
	uint8_t         pad2[4];
 
	// 0x58
	FIS_DEV_BITS	sdbfis;		// Set Device Bit FIS
 
	// 0x60
	uint8_t         ufis[64];
 
	// 0xA0
	uint8_t   	rsv[0x100-0xA0];
} HBA_FIS;

typedef struct tagHBA_CMD_HEADER
{
	// DW0
	uint8_t  cfl:5;		// Command FIS length in DWORDS, 2 ~ 16
	uint8_t  a:1;		// ATAPI
	uint8_t  w:1;		// Write, 1: H2D, 0: D2H
	uint8_t  p:1;		// Prefetchable
 
	uint8_t  r:1;		// Reset
	uint8_t  b:1;		// BIST
	uint8_t  c:1;		// Clear busy upon R_OK
	uint8_t  rsv0:1;		// Reserved
	uint8_t  pmp:4;		// Port multiplier port
 
	uint16_t prdtl;		// Physical region descriptor table length in entries
 
	// DW1
	volatile
	uint32_t prdbc;		// Physical region descriptor byte count transferred
 
	// DW2, 3
	uint32_t ctba;		// Command table descriptor base address
	uint32_t ctbau;		// Command table descriptor base address upper 32 bits
 
	// DW4 - 7
	uint32_t rsv1[4];	// Reserved
} HBA_CMD_HEADER;

typedef struct tagHBA_PRDT_ENTRY
{
	uint32_t dba;		// Data base address
	uint32_t dbau;		// Data base address upper 32 bits
	uint32_t rsv0;		// Reserved
 
	// DW3
	uint32_t dbc:22;		// Byte count, 4M max
	uint32_t rsv1:9;		// Reserved
	uint32_t i:1;		// Interrupt on completion
} HBA_PRDT_ENTRY;

typedef struct tagHBA_CMD_TBL
{
	// 0x00
	uint8_t  cfis[64];	// Command FIS
 
	// 0x40
	uint8_t  acmd[16];	// ATAPI command, 12 or 16 bytes
 
	// 0x50
	uint8_t  rsv[48];	// Reserved
 
	// 0x80
	HBA_PRDT_ENTRY	prdt_entry[1];	// Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;

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
