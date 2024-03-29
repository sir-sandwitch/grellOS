#ifndef STDINT_INCL
#include <stdint.h>
#define STDINT_INCL
#endif

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8E
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define PIT_FREQUENCY 120

#define ENTER_KEY_CODE 0x1C
#define BACK_KEY_CODE 0x0E
#define SPACE_KEY_CODE 0x39
#define NULL 0

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern void pit_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

/* current cursor location */
extern unsigned int current_loc;

extern char *vidptr;

/*Backspace flag*/
extern int bsflag;

extern char command;

extern char cmdbuf[256];

extern char color;
extern char fgcolor;
extern char bgcolor;

typedef struct registers{
    unsigned long eax;
    unsigned long ebx;
    unsigned long ecx;
    unsigned long edx;
    unsigned long esi;
    unsigned long edi;
    unsigned long ebp;
    unsigned long esp;
    unsigned long eip;
    unsigned long eflags;
}registers_t;

// typedef volatile struct tagHBA_PORT
// {
// 	uint32_t clb;		// 0x00, command list base address, 1K-byte aligned
// 	uint32_t clbu;		// 0x04, command list base address upper 32 bits
// 	uint32_t fb;		// 0x08, FIS base address, 256-byte aligned
// 	uint32_t fbu;		// 0x0C, FIS base address upper 32 bits
// 	uint32_t is;		// 0x10, interrupt status
// 	uint32_t ie;		// 0x14, interrupt enable
// 	uint32_t cmd;		// 0x18, command and status
// 	uint32_t rsv0;		// 0x1C, Reserved
// 	uint32_t tfd;		// 0x20, task file data
// 	uint32_t sig;		// 0x24, signature
// 	uint32_t ssts;		// 0x28, SATA status (SCR0:SStatus)
// 	uint32_t sctl;		// 0x2C, SATA control (SCR2:SControl)
// 	uint32_t serr;		// 0x30, SATA error (SCR1:SError)
// 	uint32_t sact;		// 0x34, SATA active (SCR3:SActive)
// 	uint32_t ci;		// 0x38, command issue
// 	uint32_t sntf;		// 0x3C, SATA notification (SCR4:SNotification)
// 	uint32_t fbs;		// 0x40, FIS-based switch control
// 	uint32_t rsv1[11];	// 0x44 ~ 0x6F, Reserved
// 	uint32_t vendor[4];	// 0x70 ~ 0x7F, vendor specific
// } HBA_PORT;

// typedef volatile struct tagHBA_MEM
// {
// 	// 0x00 - 0x2B, Generic Host Control
// 	uint32_t cap;		// 0x00, Host capability
// 	uint32_t ghc;		// 0x04, Global host control
// 	uint32_t is;		// 0x08, Interrupt status
// 	uint32_t pi;		// 0x0C, Port implemented
// 	uint32_t vs;		// 0x10, Version
// 	uint32_t ccc_ctl;	// 0x14, Command completion coalescing control
// 	uint32_t ccc_pts;	// 0x18, Command completion coalescing ports
// 	uint32_t em_loc;		// 0x1C, Enclosure management location
// 	uint32_t em_ctl;		// 0x20, Enclosure management control
// 	uint32_t cap2;		// 0x24, Host capabilities extended
// 	uint32_t bohc;		// 0x28, BIOS/OS handoff control and status
 
// 	// 0x2C - 0x9F, Reserved
// 	uint8_t  rsv[0xA0-0x2C];
 
// 	// 0xA0 - 0xFF, Vendor specific registers
// 	uint8_t  vendor[0x100-0xA0];
 
// 	// 0x100 - 0x10FF, Port control registers
// 	HBA_PORT	ports[1];	// 1 ~ 32
// } HBA_MEM;
 