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
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;

/*Backspace flag*/
int bsflag = 0;

char command = ' ';

char cmdbuf[256];

char color = 0x62;
char fgcolor = 0x2;
char bgcolor = 0x6;

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