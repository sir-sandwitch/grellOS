#include "keyboard.h"

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

#define ENTER_KEY_CODE 0x1C
#define BACK_KEY_CODE 0x0E
#define SPACE_KEY_CODE 0x39
#define NULL 0

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
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


struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];


void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    /* populate IDT entry of keyboard's interrupt */
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    /*     Ports
    *    PIC1    PIC2
    *Command 0x20    0xA0
    *Data     0x21    0xA1
    */

    /* ICW1 - begin initialization */
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);

    /* ICW3 - setup cascading */
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);

    /* ICW4 - environment info */
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    /* fill the IDT descriptor */
    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
}

void kb_init(void)
{
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    write_port(0x21 , 0xFD);
}

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

int strlen(char str[]) {
    // initializing count variable (stores the length of the string)
    int count;

    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);

    // returning the character count of the string
    return count;
}

char* strcat(char* str1, char* str2) {
    char str3[256];
    int i = 0, j = 0;

    while (str1[i] != '\0') {
        str3[j++] = str1[i++];
    }

    // Insert the second string in the new string
    i = 0;
    while (str2[i] != '\0') {
        str3[j++] = str2[i++];
    }
    str3[j] = '\0';

    // Copy the concatenated string back to str1
    for (i = 0; str3[i] != '\0'; ++i) {
        str1[i] = str3[i];
    }
    str1[i] = '\0';

    return str1;
}

char* strncat(char* str1, char* str2, int n) {
    int i, j;

    for (i = 0; str1[i] != '\0'; ++i) {
    }

    for (j = 0; j < n && str2[j] != '\0'; ++j) {
        str1[i++] = str2[j];
    }
    str1[i] = '\0';

    return str1;
}

char* strcpy(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] != '\0'; ++i) {
        s2[i] = s1[i];
    }

    s2[i] = '\0';

    return s2;
}

void reverse(char* str) {
    int length = strlen(str);
    int i, j;
    char temp;

    for (i = 0, j = length - 1; i < j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

char* ltoa(long num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Convert individual digits
    while (num != 0) {
        long remainder = num % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }

    // Append negative sign for base 10
    if (isNegative && base == 10)
        str[i++] = '-';

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    reverse(str);

    return str;
}

/*djb2 hash taken from http://www.cse.yorku.ca/~oz/hash.html*/
unsigned long hash(unsigned char* str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

void command_handler_main(char* cmd) {
    char commands[16][256] = { {0} };

    int j = 0;
    int q = 0;

    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == ' ') {
            j++;
            q = 0;
        }
        else {
            strncat(commands[j], &cmd[i], 1);
            q++;
        }
    }

    char* tmpcmd = commands[0];
    unsigned long command = hash(tmpcmd);
    unsigned long args[16];
    for (int q = 1; q < 16; q++) {
        if (commands[q][0] != '\0') {
            tmpcmd = commands[q];
            args[q - 1] = hash(tmpcmd);
        }
    }

	/*switch cmd hash*/
    switch (command) {
        case 0xF3B6D8C: //clear
			current_loc = 0;
            clear_screen();
            break;
		case 0x7C97D2EE: //help
			kprint("Available commands:");
			kprint_newline();
			kprint("clear");
			kprint_newline();
			kprint("help");
			kprint_newline();
			kprint("echo");
			kprint_newline();
		case 0x7C9624C4: //echo
			for (int i = 1; i < 16; i++) {
				if (commands[i][0] != '\0') {
					kprint(commands[i]);
					kprint(" ");
				}
			}
			kprint_newline();
			break;
		case 0x5977D2: //fg
			//set fg color
			if (args[0] != NULL) {
				switch(args[0]) {
					case 0xF294442: //black
						fgcolor = 0x0;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C94A78D: //blue
						fgcolor = 0x1;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF871A56: //green
						fgcolor = 0x2;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9568B0: //cyan
						fgcolor = 0x3;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB88A540: //red
						fgcolor = 0x4;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB4110202: //magenta
						fgcolor = 0x5;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF2CCCAD: //brown
						fgcolor = 0x6;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C977C78: //gray
						fgcolor = 0x8;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9C4737: //pink
						fgcolor = 0xD;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x297FF6E1: //yellow
						fgcolor = 0xE;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x10A33986: //white
						fgcolor = 0xF;
						color = (bgcolor << 4) | fgcolor;
						break;
					default:
						kprint("Invalid color");
						kprint_newline();
						break;
				}
			}
			else kprint("Invalid color");
			break;	

		case 0x59774E: //bg
			if(args[0] != NULL) {
				switch(args[0]){
					case 0xFDCAE5D: //light variation of color
						switch(args[1]){
							case 0x7C977C78: //gray
								bgcolor = 0x7;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0x7C94A78D: //blue
								bgcolor = 0x9;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0xF871A56: //green
								bgcolor = 0xA;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0x7C9568B0: //cyan
								bgcolor = 0xB;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							case 0xB88A540: //red
								bgcolor = 0xC;
								color = (fgcolor << 4) | bgcolor;
								color = (bgcolor << 4) | fgcolor;
								break;
							default:
								kprint("Invalid color");
								kprint_newline();
								break;
						}
						break;
					case 0xF294442: //black
						bgcolor = 0x0;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C94A78D: //blue
						bgcolor = 0x1;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF871A56: //green
						bgcolor = 0x2;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9568B0: //cyan
						bgcolor = 0x3;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB88A540: //red
						bgcolor = 0x4;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xB4110202: //magenta
						bgcolor = 0x5;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0xF2CCCAD: //brown
						bgcolor = 0x6;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C977C78: //gray
						bgcolor = 0x8;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x7C9C4737: //pink
						bgcolor = 0xD;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x297FF6E1: //yellow
						bgcolor = 0xE;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					case 0x10A33986: //white
						bgcolor = 0xF;
						color = (fgcolor << 4) | bgcolor;
						color = (bgcolor << 4) | fgcolor;
						break;
					default:
						kprint("Invalid color");
						kprint_newline();
						break;
				}
			}
			else kprint("Invalid color");
			break;

        default:
            kprint("Command not found, hash: 0x");
            char hashstr[17];
            ltoa(command, hashstr, 16);
            kprint(hashstr);
            kprint(", cmd: ");
            kprint(commands[0]);
			kprint_newline();
            break;
    }
	return;
}

void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0) {
            return;
        }

        if (keycode == ENTER_KEY_CODE) {
            //cmdbuf[strlen(cmdbuf) - 1] = '\0';
            kprint_newline();
            command_handler_main(cmdbuf);
            cmdbuf[0] = '\0';
        }
        else if (keycode == BACK_KEY_CODE) {
            if (strlen(cmdbuf) > 0) {
                cmdbuf[strlen(cmdbuf) - 1] = '\0';
                bsflag = 1;
            }
        }
        else {
            char str[2] = { keyboard_map[(unsigned char)keycode], '\0' };
            strcat(cmdbuf, str);
            kprint(str);
        }
    }
}

void kmain(void)
{
    const char *str = "grellOS v0.1";
    clear_screen();
    kprint(str);
    kprint_newline();
    kprint_newline();

    idt_init();
    kb_init();

    while (1) {
        keyboard_handler_main();
    }
}
